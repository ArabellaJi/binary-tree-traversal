// SOMETHING NEEDS TO GO HERE!  WHY NOT DO IT NOW?

#include "threaded_bst.h"
#include <climits>         // CHAR_BIT
#include <iomanip>
#include <cstdio>
#include <stack>
using namespace std;
using std::cout;
using std::endl;
typedef ThreadedBst::Iterator Iterator;

ThreadedBst::~ThreadedBst() {
  // create a iterator for using parent()
  Iterator i = Iterator(_header, (Node*) _header, Iterator::UNDEFINED);
  // last node for delete next round
  Node* lastNode = i.postsucc()._ptr;
  // current node 
  Node* current;
  
  while(true) {
    // Empty Tree 
    if(isThread(_header->_lchild)) {
      delete lastNode;
      return;
    }
    // Only have a root node case
    else if(lastNode ==_header->_lchild) {
      cout << "<Thread case: ";
      cout << "Delete " << lastNode->_key << ">";
      delete lastNode;
      // delete header
      delete _header;
      return;
    }
    // More than one root node case 
    else {
      // follow the post order to go to the next node
      i++;
      current = i.postsucc()._ptr ;
      delete lastNode;
      lastNode = current;  
    }
  }
}


void ThreadedBst::insert(string key, int value) {
  // Empty tree case
  if(isThread(_header->_lchild) ) {
    Node* node = new Node(key,
                          value,
                          makeThread(_header),
                          makeThread(_header));
    _header->_lchild= node;
  } 
  else {
    // Set current node to the root node of tree
    Node* cur = _header->_lchild;
    Node* parentNode;
    while(true) {
      // when the key of the new node is larger than that of the current node
      // and the current node does not have the right child
      if(key > cur-> _key && isThread(cur->_rchild)) {
        parentNode = cur;
        // Created node should inherit parent thread and 
        // another side child will point to the parent node
        Node* node = new Node(key,
                              value,
                              makeThread(parentNode),
                              makeThread(parentNode->_rchild)
                              );
        cur -> _rchild = node;
        break;
      }
      // when the key of the new node is smaller than that of the current node
      // and the current node does not have the left child
      else if(key < cur-> _key && isThread(cur->_lchild)) {
        parentNode = cur;
        // Created node should inherit parent thread and 
        // another side child will point to the parent node
        Node* node = new Node(key,
                              value,
                              makeThread(parentNode->_lchild),
                              makeThread(parentNode)
                              );
        cur -> _lchild = node ;
        break;
      }
      // when the key of the new node is larger than that of the current node
      // and the current node have the right child
      else if(key > cur-> _key) {
        cur = cur -> _rchild;
      }
      // when the key of the new node is smaller than that of the current node
      // and the current node has the left child
      else if(key < cur-> _key) {
        cur = cur -> _lchild;
      } 
    }
  }
}


void ThreadedBst::erase(Iterator iter) {
  Node* cur = iter._ptr;
  Iterator curIter = iter;
  Node* parent = curIter.parent()._ptr;
  // If cur has a left child or a right child,
  // which means it is not a leaf node, then it cannot be erased directly,
  // so it need to go into this while loop to do more works.
  while (!isThread(cur -> _lchild) || !isThread(cur -> _rchild)) {
    // if cur has a right child
    if (!isThread(cur -> _rchild)) {
      // before move cur, save the original cur into the parent,
      // so we get the parent of the new cur
      parent = cur;
      cur = cur->_rchild;
      // while cur has a left child, let cur equals to its left child
      while (!isThread(cur -> _lchild)) {
        parent = cur;
        cur = cur->_lchild;
      }
      // copy the key and value of the node that cur points to 
      // to the key and value of the node curIter._ptr points to
      curIter._ptr->_key = cur->_key;
      curIter._ptr->_value = cur->_value;
      // set curIter._ptr to cur
      curIter._ptr = cur;
    }
    // if cur does not have a left child but has a right child
    else if (!isThread(cur -> _lchild)) {
      // same as what in the if statement 
      parent = cur;
      cur = cur->_lchild;
      while (!isThread(cur -> _rchild)) {
        parent = cur;
        cur = cur->_rchild;
      }
      curIter._ptr->_key = cur->_key;
      curIter._ptr->_value = cur->_value;
      curIter._ptr = cur;
    }
  }
  // if cur is the left child of its parent 
  if (cur == parent->_lchild) {
    // the parent's left child becomes cur's left child
    parent->_lchild = cur->_lchild;
  }
  // if cur is the right child of its parent 
  else if (cur == parent->_rchild) {
    // the parent's right child becomes cur's right child
    parent->_rchild = cur->_rchild;
  }
  delete cur;
}


bool ThreadedBst::empty() const {
  if( isThread(_header->_lchild) ){
    return true;
  } 
  else {
    return false;
  }
  
}


Iterator ThreadedBst::lookup(string key) const {
  // empty tree case
  Node* cur = _header->_lchild;
  if(isThread(cur)) {
    return end();
  } 

  while(true) {
    // when the key of the node we are looking up
    // is larger than that of the current node
    if(key > cur -> _key) {
      if(isThread(cur -> _rchild)){
        return end();
      }
      else{
        cur =cur ->_rchild;
      }
    }
    // when the key of the node we are looking up
    // is smaller than that of the current node
    else if (key < cur -> _key) {
      if(isThread(cur -> _lchild)){
        return end();
      }
      else{
        cur =cur ->_lchild;
      }
    }
    // when the key of the node we are looking up
    // is equal to that of the current node
    else {
      return Iterator(_header, cur, Iterator::UNDEFINED);
    }
  } 

}


Iterator & Iterator::insucc() {
  // If the node has a thread for a right child, then the thread points to
  // the inorder successor; else, we go left as far as possible in the right
  // subtree

  if (isThread(_ptr -> _rchild)) {
    _ptr = makePointer(_ptr -> _rchild);
  } else {
    _ptr = _ptr -> _rchild;
    while (! isThread(_ptr -> _lchild)) {
      _ptr = _ptr -> _lchild;
    }
  }
  return * this;
}


Iterator & Iterator::inpred() {
  // If the node has a thread for a left child, then the thread points to
  // the inorder predeccessor; else, we go right as far as possible in the left
  // subtree

  if (isThread(_ptr -> _lchild)) {
    _ptr = makePointer(_ptr -> _lchild);
  } else {
    _ptr = _ptr -> _lchild;
    while (! isThread(_ptr -> _rchild)) {
      _ptr = _ptr -> _rchild;
    }
  }
  return * this;
}


Iterator & Iterator::presucc() {
  // current ptr's left child is not a thread
  if (!isThread(_ptr -> _lchild) ) {
    _ptr = _ptr -> _lchild;
  } 
  else {
    // current ptr's right child is not a thread
    if(!isThread(_ptr -> _rchild) ) { 
      _ptr = _ptr -> _rchild;
    }
    // leaf case: 
    else {
      // keep going left until the left child is not thread
      while(isThread(_ptr -> _rchild)) {
        _ptr = makePointer(_ptr -> _rchild);
      }
        _ptr = _ptr -> _rchild; 
    }
  }
  return * this;
}

Iterator & Iterator::prepred() {
  Node * parent = this->parent()._ptr;
  // This if statement is used to find the first node we want in this traversal 
  if (!isThread(_ptr -> _lchild) && _ptr->_rchild == _header) {
    _ptr = _ptr -> _lchild;
    while (! isThread(_ptr -> _rchild)) {
      _ptr = _ptr -> _rchild;
    }
    while (!isThread(_ptr -> _lchild) || !isThread(_ptr -> _rchild)) {
      if (!isThread(_ptr -> _rchild)) {
        _ptr = _ptr -> _rchild;
      } else if (!isThread(_ptr -> _lchild)) {
        _ptr = _ptr -> _lchild;
      }
    }
  }
  // current ptr is the rightchild of its parent
  if (_ptr == parent->_rchild) {
    // if the parent does not have the left child
    if (isThread(parent -> _lchild)) {
      _ptr = parent;
    }
    // if the parent has the left child
    else if (!isThread(parent -> _lchild)) {
      _ptr = parent -> _lchild;
      // while current ptr does not have right child but has left child
      while(isThread(_ptr -> _rchild) && !isThread(_ptr -> _lchild)) {
        _ptr = _ptr -> _lchild;
      }
      // while current ptr has at lease one child
      while (!isThread(_ptr -> _lchild) || !isThread(_ptr -> _rchild)) {
        // go right if it has right child
        if (!isThread(_ptr -> _rchild)) {
          _ptr = _ptr -> _rchild;
        }
        // go left if it does not have right child
        else if (!isThread(_ptr -> _lchild)) {
          _ptr = _ptr -> _lchild;
        }
      }
    }
  }
  // Current ptr is the leftchild of its parent
  else if (_ptr == parent->_lchild) {
    _ptr = parent;
  }
  return * this;
}

Iterator & Iterator::postpred() {
  // Go right child first if not thread
  if (!isThread(_ptr -> _rchild) && _ptr!=_header) {
      _ptr = _ptr -> _rchild;
  }
  // Then go left child if not thread 
  else if (!isThread(_ptr -> _lchild) ) {
    _ptr = _ptr -> _lchild;
  }
  // Leaf Node case:
  else {
    // keep going left until hit a left child is not thread
    while (isThread(_ptr -> _lchild) ) {
      _ptr = makePointer(_ptr -> _lchild);
    }
    // header case:
    if (_ptr != _header){
      // Go one more left which is not thread
      _ptr = _ptr -> _lchild;
    }
  }
  return *this;
}


Iterator Iterator::parent() const {
  Node* parent;
  // current node is root of Tree case or empty Tree 
  if(isThread(_header->_lchild)||_ptr == _header->_lchild) {
    // return the _header
    return Iterator(_header, (Node*) _header, Iterator::UNDEFINED);
  } 
  else {
    Node* current =  _header->_lchild;
    while( current->_key != _ptr->_key) {
      // First changing the parent value to current
      parent = current;
      // Smaller Case
      if(_ptr->_key < current->_key) {
        // Go left if it's a thread make it become a pointer
        if (isThread(current->_lchild) ) {
          current = makePointer(current->_lchild);
        } else {
          current = current->_lchild; 
        }   
      }
      // larger Case
      else {
        // Go right if it's a thread make it become a pointer
        if (isThread(current->_rchild) ) {
          current = makePointer(current->_rchild);
        } else {
          current = current->_rchild; 
        }   
      }
    }
  }
  // Return the Iterator with parent node
  return Iterator(_header, (Node *) parent, Iterator::UNDEFINED);
}

Iterator & Iterator::postsucc() {
  // create a iterator for using parent() 
  Iterator i = Iterator(_header, (Node *) _ptr, Iterator::UNDEFINED);
  // Parent node
  Node* parent = i.parent()._ptr;
  // For keep tracking which round cur is currently at 
  // If now is cur's round we will not loop in the left right root case 
  // but instead of going right and left. 
  // Because cur might be the root of two child and we will always loop 
  // in to left right root case forever
  Node* cur = _ptr;
  // Tree root Case:
  if(_ptr == _header->_lchild) {
    _ptr = (Node*)_header;
    return *this;
  }
  // current ptr is the rightchild of his parent
  else if (_ptr == parent->_rchild) {
    _ptr = parent;
    return *this;
  }

  // Current ptr is the leftchild of his parent
  else if (_ptr == parent->_lchild) {
    // Right child is not a thread
    if(!isThread(parent->_rchild)) {
      _ptr = parent->_rchild; 
    }
    // Current ptr's left child is not a thread and Special case 
    // Using cur to check which round we current at if it's cur round 
    // we go parent's right note and then go parent node
    else if(!isThread(_ptr -> _lchild) &&  cur != _ptr) {
      // keep going left until hit a thread
      while (!isThread(_ptr -> _lchild) ) {
        _ptr = _ptr -> _lchild;
      } 
    }
    // Right child is a thread
    else {
      _ptr = parent;
      return *this;
    }
   
  }
  // Keep going until find a leaf node 
  while(!isThread(_ptr->_lchild) || !isThread(_ptr->_rchild)) {
    // Only if the left child is a thread
    if(isThread(_ptr->_lchild)) {
      // Go right
      _ptr = _ptr->_rchild;
    }
    // Go left
    else {
      _ptr = _ptr->_lchild;
    }
  }
  return *this;
}
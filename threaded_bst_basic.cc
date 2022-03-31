#include "threaded_bst.h"
#include <climits>         // CHAR_BIT
#include <iomanip>
using std::setw;
#include <cstdio>
using std::cout;
using std::endl;

/*
 * Basic methods and helpers for class ThreadedBst and its private
 * classes.  This code is provided as part of CPS 222 Project 3,
 * and should not be changed.
 *
 * The rest of the methods for ThreadedBst (and its private classes)
 * are to be writen by students in project3.cc.
 *
 * Copyright(c) 1997, 1999, 2001, 2003, 2014 - Russell C. Bjork
 * Updates copyright (c) 2017, 2019 - Russ Tuck
 */

typedef ThreadedBst::Iterator Iterator;


ThreadedBst::ThreadedBst()
: _header(new  Node())
{
    _header->_lchild = makeThread(_header);
    _header->_rchild = _header;
}


Iterator ThreadedBst::inorder_begin() const {
    return Iterator(_header, _header, Iterator::INORDER).insucc();
}

Iterator ThreadedBst::inorder_rbegin()const {
    return Iterator(_header, _header, Iterator::INORDER).inpred();
}

Iterator ThreadedBst::preorder_begin() const {
    return Iterator(_header, _header, Iterator::PREORDER).presucc();
}

Iterator ThreadedBst::preorder_rbegin() const {
    return Iterator(_header, _header, Iterator::PREORDER).prepred();
}

Iterator ThreadedBst::postorder_begin() const {
    return Iterator(_header, _header, Iterator::POSTORDER).postsucc();
}

Iterator ThreadedBst::postorder_rbegin() const {
    return Iterator(_header, _header, Iterator::POSTORDER).postpred();
}

Iterator ThreadedBst::end() const {
    return Iterator(_header, _header, Iterator::UNDEFINED);
}

/*
 * Thread manipulation methods
 *
 */

// This should work for both 32- and 64-bit processors
#define THREAD_FLAG_MASK (1ll << (sizeof(void *)*CHAR_BIT - 1))

bool ThreadedBst::isThread(Node *ptr) {
    return (((long long) ptr) & THREAD_FLAG_MASK) != 0;
}

ThreadedBst::Node *ThreadedBst::makeThread(Node *ptr) {
    return (Node *) (((long long) ptr) | THREAD_FLAG_MASK);
}

ThreadedBst::Node * ThreadedBst::makePointer(Node *thread) {
    return (Node *) (((long long) thread) & ~THREAD_FLAG_MASK);
}


/*
 * Storage allocation/deallocation for node
 * Checks whether allocations and deallocations match.
 */

// Initialize static variables for checking correct node allocation/deallocation
int ThreadedBst::newCount = 0;
int ThreadedBst::deleteCount = 0;
void *ThreadedBst::lastNodeAllocated = NULL;


void *ThreadedBst::Node::operator new(size_t size) {
    newCount ++;
    lastNodeAllocated = ::operator new(size);
    return lastNodeAllocated;
}


void ThreadedBst::Node::operator delete(void *ptr) {
    deleteCount ++;
    ::operator delete(ptr);
}


string ThreadedBst::getLastNodeAllocated() {
    char buffer[20];
    sprintf(buffer, "%-18p", lastNodeAllocated);
    return (string) buffer;
}


bool ThreadedBst::report() {
    cout << "You created a total of: " << newCount << " nodes" << endl;
    cout << "You deleted a total of: " << deleteCount << " nodes" << endl;
    if (newCount == deleteCount) {
        cout << "You are a good steward of nodes!" << endl;
        return true;
    } else if (newCount < deleteCount) {
        cout << "WARNING: YOU DELETED SOME NODE(S) MORE THAN ONCE!" << endl;
    } else {
        cout << "LITTERBUG!" << endl;
    }
    return false;
}


bool ThreadedBst::checkMem() {
    if (newCount == deleteCount) {
        return true;
    } else if (newCount < deleteCount) {
        cout << "WARNING: YOU DELETED SOME NODE(S) MORE THAN ONCE!" << endl;
    } else {
        cout << "LITTERBUG!" << endl;
    }
    cout << "You created a total of: " << newCount << " nodes" << endl;
    cout << "You deleted a total of: " << deleteCount << " nodes" << endl;
    return false;
}


/*
 * Methods of class ThreadedBst::Iterator
 */

Iterator::Iterator()
  : _header(NULL), _ptr(NULL), _order(UNDEFINED) {
}


Iterator & Iterator::operator ++() {
    switch (_order)
    {
        case INORDER:   return insucc();
        case PREORDER:  return presucc();
        case POSTORDER: return postsucc();
        default:        return *this;
    }
}


Iterator & Iterator::operator --() {
    switch (_order)
    {
        case INORDER:   return inpred();
        case PREORDER:  return prepred();
        case POSTORDER: return postpred();
        default:        return *this;
    }
}


Iterator Iterator::operator ++(int) {
    Iterator result = *this;
    operator ++();
    return result;
}


Iterator Iterator::operator --(int) {
    Iterator result = *this;
    operator --();
    return result;
}


string Iterator::key() const {
    return _ptr->_key;
}


int Iterator::value() const {
    return _ptr->_value;
}


bool Iterator::operator == (const Iterator & other) const {
    return _ptr == other._ptr && _ptr != NULL;
}


bool Iterator::operator != (const Iterator & other) const {
    return _ptr != other._ptr || _ptr == NULL;
}


Iterator::Iterator(const Node *header,
                   Node *ptr,
                   Order order)
  : _header(header), _ptr(ptr), _order(order) {
}


ostream & operator << (ostream & stream,
                       ThreadedBst::Iterator & iter) {
    char buffer[80];

    sprintf(buffer, "%-18p ", (void *) iter._ptr);
    stream << buffer;
    if (iter._ptr == NULL)
        stream << "*** NULL - THIS IS A VERY BAD THING! ***";
    else if (iter._ptr == iter._header)
        stream << "*** HEADER ***";
    else if (ThreadedBst::isThread(iter._ptr))
        stream << "*** THREAD ***";
    else
    {
        stream << iter._ptr->_key
               << setw((iter._ptr->_key.length() > 14) ? 1
                       : 14 - iter._ptr->_key.length()) << " "
               << " - value: " <<  setw(3) << iter._ptr->_value;
        stream << " - children: ";

        if (ThreadedBst::isThread(iter._ptr->_lchild))
            if (ThreadedBst::makePointer(iter._ptr->_lchild)
                == iter._header)
                stream << "thr header            ";
            else
            {
                sprintf(buffer, "thr %-18p",
                        (void *) ThreadedBst::makePointer(
                            iter._ptr->_lchild));
                stream << buffer;
            }
        else
            if (iter._ptr->_lchild == iter._header)
                stream << "    header            ";
            else
            {
                sprintf(buffer, "    %-18p", (void *) iter._ptr->_lchild);
                stream << buffer;
            }

        stream << " ";

        if (ThreadedBst::isThread(iter._ptr->_rchild))
            if (ThreadedBst::makePointer(iter._ptr->_rchild)
                == iter._header)
                stream << "thr header            ";
            else
            {
                sprintf(buffer, "thr %-18p",
                        (void *) ThreadedBst::makePointer(
                            iter._ptr->_rchild));
                stream << buffer;
            }
        else
            if (iter._ptr->_rchild == iter._header)
                stream << "    header            ";
            else
            {
                sprintf(buffer, "    %-18p", (void *) iter._ptr->_rchild);
                stream << buffer;
            }
    }
    return stream;
}

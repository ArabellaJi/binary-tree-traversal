/*
 * Test driver for CPS222 Project 3
 *
 * Copyright (c) 1997, 2001, 2002, 2003, 2013  - Russell C. Bjork
 * Updates copyright (c) 2017, 2019 - Russ Tuck
 */

#include "threaded_bst.h"
#include <cstdio>
using std::cin;
using std::cout;
using std::endl;
#include <sys/stat.h>

/*
 * Main interactive test driver program
 */

// Possible traversal orders

#define TRAVERSE_INORDER 1
#define TRAVERSE_PREORDER 2
#define TRAVERSE_POSTORDER 4

int main() {
  // Wrap the bulk of the test driver in a block, so that the destructor
  // for theTree will be called before we check to see if all nodes
  // created have been deleted
  ThreadedBst theTree;
  ThreadedBst::Iterator iter;

  // Find out whether our input stream is interactive by seeing if device
  // 0 is a character special device.  interactive is true if it is.
  struct stat s;
  fstat(0, & s);
  bool interactive = S_ISCHR(s.st_mode);

  // Commands and arguments inputted by the user
  char command;
  string key;
  int value;
  int order;
  string comment;

  do {
    if (interactive)
      cout << endl << "Command? ";
    cin >> command;
    if (cin.eof()) {
      command = 'Q';
    }

    switch(command) {
    case '?':

      // Test of empty() method

      cout << "Testing empty() - result is: "
           << (theTree.empty() ? "true" : "false") << endl;

      break;

    case 'I': case 'i': {
      // Test of insert() method

      cin >> key >> value;
      theTree.insert(key, value);

      cout << "Inserted " << key << " with value " << value << " at ";
      cout << ThreadedBst::getLastNodeAllocated()
           << endl;

      break;
    }

    case 'T' : case 't':

      cin >> order;

      // Test of forward traversal(s)

      if (order & TRAVERSE_INORDER) {
        cout << "Doing inorder traversal: " << endl << endl;
        for (iter = theTree.inorder_begin();
             iter != theTree.end(); iter ++)
          cout << iter << endl;
        cout << endl;
      }

      if (order & TRAVERSE_PREORDER) {
        cout << "Doing preorder traversal: " << endl << endl;
        for (iter = theTree.preorder_begin();
             iter != theTree.end(); iter ++)
          cout << iter << endl;
        cout << endl;
      }

      if (order & TRAVERSE_POSTORDER) {
        cout << "Doing postorder traversal: " << endl << endl;
        for (iter = theTree.postorder_begin();
             iter != theTree.end(); iter ++)
          cout << iter << endl;
        cout << endl;
      }

      break;

    case 'R': case 'r':

      cin >> order;

      // Test of reverse traversal(s)

      if (order & TRAVERSE_INORDER) {
        cout << "Doing reverse inorder traversal: "
             << endl << endl;
        for (iter = theTree.inorder_rbegin();
             iter != theTree.end(); iter --)
          cout << iter << endl;
        cout << endl;
      }

      if (order & TRAVERSE_PREORDER) {
        cout << "Doing reverse preorder traversal: "
             << endl << endl;
        for (iter = theTree.preorder_rbegin();
             iter != theTree.end(); iter --)
          cout << iter << endl;
        cout << endl;
      }

      if (order & TRAVERSE_POSTORDER) {
        cout << "Doing reverse postorder traversal: "
             << endl << endl;
        for (iter = theTree.postorder_rbegin();
             iter != theTree.end();iter --)
          cout << iter << endl;
        cout << endl;
      }

      break;

    case 'L': case 'l':

      // Test of lookup

      cin >> key;

      cout << "Looking up: " << key;
      iter = theTree.lookup(key);
      if (iter == theTree.end()) {
        cout << " - not found" << endl;
      } else {
        cout << " - got:" << endl << iter << endl;
      }
      break;

    case 'P': case 'p':

      // Test of parent - must look up first

      cin >> key;

      cout << "Looking up: " << key;
      iter = theTree.lookup(key);
      if (iter == theTree.end()) {
        cout << " - key not found" << endl;
      } else {
        cout << " - key was found" << endl;
      }
      cout << "Calling parent() on result";
      iter = iter.parent();
      cout << " - got:" << endl << iter << endl;

      break;

    case 'E': case 'e':

      // Test of erase

      cin >> key;

      cout << "Looking up: " << key;
      iter = theTree.lookup(key);
      if (iter == theTree.end()) {
        cout << " - not found" << endl;
      } else {
        cout << " - found - erasing it" << endl;
        theTree.erase(iter);
      }

      break;

    case 'Q': case 'q':

      // Quit

      cout << "Quitting" << endl;

      break;

    case '#':

      // Comment

      getline(cin, comment);
      cout << "#" << comment << endl;

      break;

    default:

      cout << "Unrecognized command: " << command << endl;

    case 'H': case 'h':

      cout << "Recognized commands are: " << endl
           << "     ?(isempty); " << endl
           << "     I(nsert) key value; " << endl
           << "     T(raverse) order;" << endl
           << "     R(everse traverse) order; " << endl
           << "     L(ookup) key; " << endl
           << "     P(arent) key;" << endl
           << "     E(rase) key;" << endl
           << "     H(elp);" << endl
           << "     Q(uit) [or eof]"<< endl;
    }

  } while (command != 'Q' && command != 'q');

  ThreadedBst::report();

  return 0;
}

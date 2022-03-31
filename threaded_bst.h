/*
 * project3.h - Declarations for class ThreadedBst
 * CPS 222, Gordon College, Project 3
 *
 * Copyright(c) 1997, 1999, 2001, 2003, 2014 - Russell C. Bjork
 * Updates copyright (c) 2017, 2019 - Russ Tuck
 */

#include <iostream>
using std::ostream;
#include <string>
using std::string;

/* ThreadedBst: Store and access key-value pairs consisting of a
 * string key and an integer value, using a binary search tree.
 * Operations include insert, lookup, and erase.  Iterators provide
 * forward and backward traversals using inorder, preorder, and
 * postorder orderings.
 */
class ThreadedBst
{
  public:

    class Iterator;			    // Fully declared below

    /* Constructor & destructor.
     * The rest of the "big 3" are handled in a private section below.
     */
    ThreadedBst();
    ~ThreadedBst();

    /* Mutators */

    // Insert a node with key and value at the proper place in the tree.
    void insert(string key, int value);

    // Remove the node pointed to by iter
    void erase(Iterator iter);

    /* Accessors */

    // Return true if and only if the tree is empty
    bool empty() const;

    // Find a node matching key in the tree, and return an iterator
    // pointing to that node.  If there is no node with that key, return
    // the iterator end().
    Iterator lookup(string key) const;

    // The following methods return iterators that support forward and reverse
    // traversal of the tree in the various orders by using operator ++/--
    // or appropriate __succ (successor) or __pred (predecessor) methods of
    // the Iterator.  The xxx_begin() form returns an iterator to the first
    // node in the specified order; the xxx_rbegin() returns an iterator to
    // the last node in the specified order - i.e. the first in reverse order.
    Iterator inorder_begin() const;
    Iterator inorder_rbegin() const;
    Iterator preorder_begin() const;
    Iterator preorder_rbegin() const;
    Iterator postorder_begin() const;
    Iterator postorder_rbegin() const;

    // The following value is returned by all iterators when advanced
    // past the last, or backed up past the first, node in the tree in
    // the respective order. It is also returned by failed lookup() and
    // parent() operations
    Iterator end() const;

    // Methods to support debugging with info about Node allocations.

    // Return string with printable address of Node allocated.
    // This should be used for reporting and debugging only.
    static string getLastNodeAllocated();

    // Prints report about balance of Node creations and deletions.
    // Returns true iff they are equal.
    static bool report();

    // Checks whether Node creations and deletions are balanced.
    // Returns true if they are equal.
    // Otherwise, prints error message and returns false.
    static bool checkMem();

  private:

    // Copy constructor, assignment operator (rest of "Big 3" after destructor):
    // Implementing these methods correctly would require a deep copy of the
    // entire tree.  Rather than doing this (which would be very time
    // consuming to carry out) we prohibit the use of these operations by
    // declaring the methods private.
    ThreadedBst(const ThreadedBst &);
    const ThreadedBst
        & operator = (const ThreadedBst &);

    // Static variables to help check correct node allocation/deallocation
    static int newCount;
    static int deleteCount;
    static void *lastNodeAllocated;

    // Incomplete declaration for class Node - needed by Iterator
    class Node;

  public:

    /* Nested class Iterator */

    class Iterator
    {
      public:

        // Public Constructor

        Iterator();

        // Move forward and backward in tree in appropriate order,
        // and return the modified iterator
        Iterator & operator ++();	// Use order specified when
        Iterator & operator --();	// the Iterator was created

        Iterator operator ++(int);	// Post-increment: x++
        Iterator operator --(int);	// Post-decrement: x--

        // See comment above inorder_begin() for important details.
        Iterator & insucc();            // In-order Successor
        Iterator & inpred();            // In-order Predecessor
        Iterator & presucc();           // Pre-order Successor
        Iterator & prepred();           // Pre-order Predecessor
        Iterator & postsucc();          // Post-order Successor
        Iterator & postpred();          // Post-order Predecessor

        // Return iterator to parent of current node, or end() if none
        Iterator parent() const;

        // Access info in node selected by iterator
        string key() const;
        int value() const;

        // Comparison iterators - equal if at same node
        bool operator == (const Iterator & other) const;
        bool operator != (const Iterator & other) const;

        // Output info on node selected by iterator (used by tester)
        friend ostream & operator << (ostream & s, Iterator & it);

      private:

        friend class ThreadedBst;

        // Iterators are available for three traversal orders
        enum Order { UNDEFINED, INORDER, PREORDER, POSTORDER };

        // Constructor creates an iterator
        // Args:
        //   header - header of the tree the iterator works within
        //   ptr - identifies the initial node in the tree for the iterator
        //   order - defines the traveral order to use
        Iterator(const Node * header,
                 Node * ptr,
                 Order order);

        // Header of the tree for which this is an iterator,
        const Node *_header;

        // Node in the tree that iterator currently refers to,
        Node *_ptr;

        // Traversal order for this iterator
        Order _order;
    };

    friend class Iterator;

  private:

    // Nodes of this type are used to actually store the tree
    class Node
    {
    public:

        Node()
        { }

        Node(string key, int value, Node * lchild, Node * rchild)
            : _key(key), _value(value), _lchild(lchild), _rchild(rchild)
        { }

        ~Node()
        { }

        string	_key;
        int	_value;
        Node *	_lchild,
            *	_rchild;

        // The following are implemented in the test driver - they
        // let the tester keep track of storage allocation/deallocation
        void * operator new(size_t size);
        void operator delete(void * ptr);
    };

    // The following functions support threading the tree

    // Return true if and only if the "pointer" ptr is a thread
    static bool isThread(Node * ptr);

    // Return a thread equivalent to ptr
    static Node *makeThread(Node * ptr);

    // Return a pointer equivalent to thread
    static Node *makePointer(Node * thread);

    friend ostream & operator << (ostream & s, Iterator & it);

    // Header node through which the tree is accessed
    Node *_header;
};

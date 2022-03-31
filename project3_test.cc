#include "gtest/gtest.h"
#include "threaded_bst.h"


TEST(lookup, insert1Lookup) {
    ThreadedBst tree;
    tree.insert("someKey", 10);
    ASSERT_NE(tree.end(), tree.lookup("someKey"));
    EXPECT_EQ(10, tree.lookup("someKey").value());
}


TEST(lookup, lookupEmpty) {
    ThreadedBst tree;
    ASSERT_EQ(tree.end(), tree.lookup("someKey"));
}


TEST(lookup, insert2LookupMissing) {
    ThreadedBst tree;
    tree.insert("someKey", 10);
    tree.insert("otherKey", 20);
    ASSERT_EQ( tree.end(), tree.lookup("anotherKey") );
}



TEST(erase, eraseRoot) {
    ThreadedBst tree;
    tree.insert("M", 2);
    tree.insert("O", 3);
    tree.insert("U", 5);
    tree.insert("S", 4);
    tree.insert("E", 1);
    tree.erase(tree.lookup("M"));
    ThreadedBst::Iterator i = tree.preorder_begin();
    EXPECT_EQ(3, i.value()); //E
    i++;
    EXPECT_EQ(1, i.value()); //O
    i++;
    EXPECT_EQ(4, i.value()); //U
    i++;
    EXPECT_EQ(5, i.value()); //S
    EXPECT_EQ(tree.end(), i.presucc()); //header
}

TEST(erase, eraseLeaf) {
    ThreadedBst tree;
    tree.insert("M", 2);
    tree.insert("O", 3);
    tree.insert("U", 5);
    tree.insert("S", 4);
    tree.insert("E", 1);
    tree.erase(tree.lookup("E"));
    ThreadedBst::Iterator i = tree.preorder_begin();
    EXPECT_EQ(2, i.value()); //E
    i++;
    EXPECT_EQ(3, i.value()); //O
    i++;
    EXPECT_EQ(5, i.value()); //U
    i++;
    EXPECT_EQ(4, i.value()); //S
    EXPECT_EQ(tree.end(), i.presucc()); //header
}

TEST(erase, eraseMid) {
    ThreadedBst tree;
    tree.insert("O", 3);
    tree.insert("S", 4);
    tree.insert("U", 5);
    tree.insert("E", 1);
    tree.erase(tree.lookup("S"));
    ThreadedBst::Iterator i = tree.preorder_begin();
    EXPECT_EQ(3, i.value()); //O
    i++;
    EXPECT_EQ(1, i.value()); //U
    i++;
    EXPECT_EQ(5, i.value()); //S
    EXPECT_EQ(tree.end(), i.presucc()); //header
}

TEST(erase, eraseMore) {
    ThreadedBst tree;
    tree.insert("M", 2);
    tree.insert("O", 3);
    tree.insert("U", 5);
    tree.insert("S", 4);
    tree.insert("E", 1);
    tree.erase(tree.lookup("M"));
    tree.erase(tree.lookup("S"));
    ThreadedBst::Iterator i = tree.preorder_begin();
    EXPECT_EQ(3, i.value()); //E
    i++;
    EXPECT_EQ(1, i.value()); //O
    i++;
    EXPECT_EQ(5, i.value()); //S
    EXPECT_EQ(tree.end(), i.presucc()); //header
}

TEST(erase, eraseAll) {
    ThreadedBst tree;
    tree.insert("M", 2);
    tree.insert("O", 3);
    tree.insert("U", 5);
    tree.insert("S", 4);
    tree.insert("E", 1);
    tree.erase(tree.lookup("M"));
    tree.erase(tree.lookup("O"));
    tree.erase(tree.lookup("u"));
    tree.erase(tree.lookup("S"));
    tree.erase(tree.lookup("E"));
    ThreadedBst::Iterator i = tree.preorder_begin();
    EXPECT_EQ(tree.end(), i.presucc()); //header
}

TEST(inorderTraverse, insertMouse) {
    ThreadedBst tree;
    tree.insert("M", 2);
    tree.insert("O", 3);
    tree.insert("U", 5);
    tree.insert("S", 4);
    tree.insert("E", 1);
    ThreadedBst::Iterator i = tree.inorder_begin();
    EXPECT_EQ(1, i.value()); //E
    i++;
    EXPECT_EQ(2, i.value()); //M
    i++;
    EXPECT_EQ(3, i.value()); //O
    i++;
    EXPECT_EQ(4, i.value()); //S
    i++;
    EXPECT_EQ(5, i.value()); //U
    EXPECT_EQ(tree.end(), i.insucc()); //header
}


// TODO: Write more unit tests until it tests project3.cc well.

TEST(inorderTraverse2, insertMouse) {
    ThreadedBst tree;
    tree.insert("M", 2);
    tree.insert("O", 3);
    tree.insert("U", 5);
    tree.insert("S", 4);
    tree.insert("E", 1);
    ThreadedBst::Iterator i = tree.inorder_rbegin();
    EXPECT_EQ(5, i.value()); //U
    i--;
    EXPECT_EQ(4, i.value()); //S
    i--;
    EXPECT_EQ(3, i.value()); //O
    i--;
    EXPECT_EQ(2, i.value()); //M
    i--;
    EXPECT_EQ(1, i.value()); //E
    EXPECT_EQ(tree.end(), i.inpred()); //header
}


TEST(preorderTraverse, insertMouse) {
    ThreadedBst tree;
    tree.insert("M", 2);
    tree.insert("O", 3);
    tree.insert("U", 5);
    tree.insert("S", 4);
    tree.insert("E", 1);
    ThreadedBst::Iterator i = tree.preorder_begin();
    EXPECT_EQ(2, i.value()); //M
    i++;
    EXPECT_EQ(1, i.value()); //E
    i++;
    EXPECT_EQ(3, i.value()); //O
    i++;
    EXPECT_EQ(5, i.value()); //U
    i++;
    EXPECT_EQ(4, i.value()); //S
    EXPECT_EQ(tree.end(), i.presucc()); //header
}


TEST(postorderTraverse, insertMouse) {
    ThreadedBst tree;
    tree.insert("M", 2);
    tree.insert("O", 3);
    tree.insert("U", 5);
    tree.insert("S", 4);
    tree.insert("E", 1);
    ThreadedBst::Iterator i = tree.postorder_rbegin();
    EXPECT_EQ(2, i.value()); //M
    i--;
    EXPECT_EQ(3, i.value()); //O
    i--;
    EXPECT_EQ(5, i.value()); //U
    i--;
    EXPECT_EQ(4, i.value()); //S
    i--;
    EXPECT_EQ(1, i.value()); //E
    EXPECT_EQ(tree.end(), i.postpred()); //header
}

TEST(postorderTraverse1, insertMouse) {
    ThreadedBst tree;
    tree.insert("M", 2);
    tree.insert("O", 3);
    tree.insert("U", 5);
    tree.insert("S", 4);
    tree.insert("E", 1);
    ThreadedBst::Iterator i = tree.postorder_begin();
    EXPECT_EQ(1, i.value()); //E
    i++;
    EXPECT_EQ(4, i.value()); //S
    i++;
    EXPECT_EQ(5, i.value()); //U
    i++;
    EXPECT_EQ(3, i.value()); //O
    i++;
    EXPECT_EQ(2, i.value()); //M
    EXPECT_EQ(tree.end(), i.postsucc()); //header
}

TEST(preorderTraverse1, insertMouse2) {
    ThreadedBst tree;
    tree.insert("M", 2);
    tree.insert("O", 3);
    tree.insert("U", 5);
    tree.insert("S", 4);
    tree.insert("E", 1);
    ThreadedBst::Iterator i = tree.preorder_rbegin();
    EXPECT_EQ(4, i.value()); //M
    i--;
    EXPECT_EQ(5, i.value()); //E
    i--;
    EXPECT_EQ(3, i.value()); //O
    i--;
    EXPECT_EQ(1, i.value()); //U
    i--;
    EXPECT_EQ(2, i.value()); //S
    EXPECT_EQ(tree.end(), i.prepred()); //header
}

// complicated test case:
/*
    Graphic of the Test Tree
            M
         /      \
        D        U
      /   \       \ 
    A      I       X
     \     /      / \
      C   F      V   Z  
     /   / \      \
    B   E   G      W
*/
TEST(inorderTraverse3, insertAtoZ) {
    ThreadedBst tree;
    tree.insert("M", 9);
    tree.insert("D", 4);
    tree.insert("U", 10);
    tree.insert("A", 1);
    tree.insert("I", 8);
    tree.insert("X", 13);
    tree.insert("C", 3);
    tree.insert("F", 6);
    tree.insert("V", 11);
    tree.insert("Z", 14);
    tree.insert("B", 2);
    tree.insert("E", 5);
    tree.insert("G", 7);
    tree.insert("W", 12);
    ThreadedBst::Iterator i = tree.inorder_begin();
    EXPECT_EQ(1, i.value()); //A
    i++;
    EXPECT_EQ(2, i.value()); //B
    i++;
    EXPECT_EQ(3, i.value()); //C
    i++;
    EXPECT_EQ(4, i.value()); //D
    i++;
    EXPECT_EQ(5, i.value()); //E
    i++;
    EXPECT_EQ(6, i.value()); //F
    i++;
    EXPECT_EQ(7, i.value()); //G
    i++;
    EXPECT_EQ(8, i.value()); //I
    i++;
    EXPECT_EQ(9, i.value()); //M
    i++;
    EXPECT_EQ(10, i.value()); //U
    i++;
    EXPECT_EQ(11, i.value()); //V
    i++;
    EXPECT_EQ(12, i.value()); //W
    i++;
    EXPECT_EQ(13, i.value()); //X
    i++;
    EXPECT_EQ(14, i.value()); //Z
    EXPECT_EQ(tree.end(), i.insucc()); //header
}

TEST(inorderTraverse4, insertAtoZ) {
    ThreadedBst tree;
    tree.insert("M", 9);
    tree.insert("D", 4);
    tree.insert("U", 10);
    tree.insert("A", 1);
    tree.insert("I", 8);
    tree.insert("X", 13);
    tree.insert("C", 3);
    tree.insert("F", 6);
    tree.insert("V", 11);
    tree.insert("Z", 14);
    tree.insert("B", 2);
    tree.insert("E", 5);
    tree.insert("G", 7);
    tree.insert("W", 12);
    ThreadedBst::Iterator i = tree.inorder_rbegin();
    EXPECT_EQ(14, i.value()); //Z
    i--;
    EXPECT_EQ(13, i.value()); //X
    i--;
    EXPECT_EQ(12, i.value()); //W
    i--;
    EXPECT_EQ(11, i.value()); //V
    i--;
    EXPECT_EQ(10, i.value()); //U
    i--;
    EXPECT_EQ(9, i.value()); //M
    i--;
    EXPECT_EQ(8, i.value()); //I
    i--;
    EXPECT_EQ(7, i.value()); //G
    i--;
    EXPECT_EQ(6, i.value()); //F
    i--;
    EXPECT_EQ(5, i.value()); //E
    i--;
    EXPECT_EQ(4, i.value()); //D
    i--;
    EXPECT_EQ(3, i.value()); //C
    i--;
    EXPECT_EQ(2, i.value()); //B
    i--;
    EXPECT_EQ(1, i.value()); //A
    EXPECT_EQ(tree.end(), i.inpred()); //header
}

TEST(preorderTraverse3, insertAtoZ) {
    ThreadedBst tree;
    tree.insert("M", 1);
    tree.insert("D", 2);
    tree.insert("U", 10);
    tree.insert("A", 3);
    tree.insert("I", 6);
    tree.insert("X", 11);
    tree.insert("C", 4);
    tree.insert("F", 7);
    tree.insert("V", 12);
    tree.insert("Z", 14);
    tree.insert("B", 5);
    tree.insert("E", 8);
    tree.insert("G", 9);
    tree.insert("W", 13);
    ThreadedBst::Iterator i = tree.preorder_begin();
    EXPECT_EQ(1, i.value()); //M
    i++;
    EXPECT_EQ(2, i.value()); //D
    i++;
    EXPECT_EQ(3, i.value()); //A
    i++;
    EXPECT_EQ(4, i.value()); //C
    i++;
    EXPECT_EQ(5, i.value()); //B
    i++;
    EXPECT_EQ(6, i.value()); //I
    i++;
    EXPECT_EQ(7, i.value()); //F
    i++;
    EXPECT_EQ(8, i.value()); //E
    i++;
    EXPECT_EQ(9, i.value()); //G
    i++;
    EXPECT_EQ(10, i.value()); //U
    i++;
    EXPECT_EQ(11, i.value()); //X
    i++;
    EXPECT_EQ(12, i.value()); //V
    i++;
    EXPECT_EQ(13, i.value()); //W
    i++;
    EXPECT_EQ(14, i.value()); //Z
    EXPECT_EQ(tree.end(), i.presucc()); //header
}

TEST(preorderTraverse4, insertAtoZ) {
    ThreadedBst tree;
    tree.insert("M", 1);
    tree.insert("D", 2);
    tree.insert("U", 10);
    tree.insert("A", 3);
    tree.insert("I", 6);
    tree.insert("X", 11);
    tree.insert("C", 4);
    tree.insert("F", 7);
    tree.insert("V", 12);
    tree.insert("Z", 14);
    tree.insert("B", 5);
    tree.insert("E", 8);
    tree.insert("G", 9);
    tree.insert("W", 13);
    ThreadedBst::Iterator i = tree.preorder_rbegin();
    EXPECT_EQ(14, i.value()); //Z
    i--;
    EXPECT_EQ(13, i.value()); //X
    i--;
    EXPECT_EQ(12, i.value()); //W
    i--;
    EXPECT_EQ(11, i.value()); //V
    i--;
    EXPECT_EQ(10, i.value()); //U
    i--;
    EXPECT_EQ(9, i.value()); //M
    i--;
    EXPECT_EQ(8, i.value()); //I
    i--;
    EXPECT_EQ(7, i.value()); //G
    i--;
    EXPECT_EQ(6, i.value()); //F
    i--;
    EXPECT_EQ(5, i.value()); //E
    i--;
    EXPECT_EQ(4, i.value()); //D
    i--;
    EXPECT_EQ(3, i.value()); //C
    i--;
    EXPECT_EQ(2, i.value()); //B
    i--;
    EXPECT_EQ(1, i.value()); //A
    EXPECT_EQ(tree.end(), i.prepred()); //header
}

TEST(postorderTraverse3, insertAtoZ) {
    ThreadedBst tree;
    tree.insert("M", 14);
    tree.insert("D", 8);
    tree.insert("U", 13);
    tree.insert("A", 3);
    tree.insert("I", 7);
    tree.insert("X", 12);
    tree.insert("C", 2);
    tree.insert("F", 6);
    tree.insert("V", 10);
    tree.insert("Z", 11);
    tree.insert("B", 1);
    tree.insert("E", 4);
    tree.insert("G", 5);
    tree.insert("W", 9);
    ThreadedBst::Iterator i = tree.postorder_begin();
    EXPECT_EQ(1, i.value()); //A
    i++;
    EXPECT_EQ(2, i.value()); //B
    i++;
    EXPECT_EQ(3, i.value()); //C
    i++;
    EXPECT_EQ(4, i.value()); //D
    i++;
    EXPECT_EQ(5, i.value()); //E
    i++;
    EXPECT_EQ(6, i.value()); //F
    i++;
    EXPECT_EQ(7, i.value()); //G
    i++;
    EXPECT_EQ(8, i.value()); //I
    i++;
    EXPECT_EQ(9, i.value()); //M
    i++;
    EXPECT_EQ(10, i.value()); //U
    i++;
    EXPECT_EQ(11, i.value()); //V
    i++;
    EXPECT_EQ(12, i.value()); //W
    i++;
    EXPECT_EQ(13, i.value()); //X
    i++;
    EXPECT_EQ(14, i.value()); //Z
    EXPECT_EQ(tree.end(), i.postsucc()); //header
}

TEST(preorderTraverse2, insertMouse2) {
    ThreadedBst tree;
    tree.insert("M", 2);
    tree.insert("O", 3);
    tree.insert("U", 5);
    tree.insert("S", 4);
    tree.insert("E", 1);
    ThreadedBst::Iterator i = tree.preorder_rbegin();
    EXPECT_EQ(4, i.value()); //M
    i--;
    EXPECT_EQ(5, i.value()); //E
    i--;
    EXPECT_EQ(3, i.value()); //O
    i--;
    EXPECT_EQ(1, i.value()); //U
    i--;
    EXPECT_EQ(2, i.value()); //S
    EXPECT_EQ(tree.end(), i.prepred()); //header
}


TEST(postorderTraverse4, insertAtoZ) {
    ThreadedBst tree;
    tree.insert("M", 14);
    tree.insert("D", 8);
    tree.insert("U", 13);
    tree.insert("A", 3);
    tree.insert("I", 7);
    tree.insert("X", 12);
    tree.insert("C", 2);
    tree.insert("F", 6);
    tree.insert("V", 10);
    tree.insert("Z", 11);
    tree.insert("B", 1);
    tree.insert("E", 4);
    tree.insert("G", 5);
    tree.insert("W", 9);
    ThreadedBst::Iterator i = tree.postorder_rbegin();
    EXPECT_EQ(14, i.value()); //Z
    i--;
    EXPECT_EQ(13, i.value()); //X
    i--;
    EXPECT_EQ(12, i.value()); //W
    i--;
    EXPECT_EQ(11, i.value()); //V
    i--;
    EXPECT_EQ(10, i.value()); //U
    i--;
    EXPECT_EQ(9, i.value()); //M
    i--;
    EXPECT_EQ(8, i.value()); //I
    i--;
    EXPECT_EQ(7, i.value()); //G
    i--;
    EXPECT_EQ(6, i.value()); //F
    i--;
    EXPECT_EQ(5, i.value()); //E
    i--;
    EXPECT_EQ(4, i.value()); //D
    i--;
    EXPECT_EQ(3, i.value()); //C
    i--;
    EXPECT_EQ(2, i.value()); //B
    i--;
    EXPECT_EQ(1, i.value()); //A
    EXPECT_EQ(tree.end(), i.postpred()); //header
}

/* Set up global teardown to assert that allocation/deallocation
 * counts match.
 */
class P3env: public ::testing::Environment {
  public:
    void TearDown();
};

// After all tests, check that tree Nodes were allocated and deleted correctly
void P3env::TearDown() {
    ASSERT_TRUE(ThreadedBst::checkMem());
}

// This gets called before main() runs, which is important, and arranges
// for SetUp() and TearDown() to be called before and after all tests.
// https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md
::testing::Environment* const p3_env =
                          ::testing::AddGlobalTestEnvironment(new P3env);

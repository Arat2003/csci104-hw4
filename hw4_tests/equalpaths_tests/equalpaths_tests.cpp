//
// CS104 internal-range-gtest
//
#include <gtest/gtest.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "equal-paths.h"

using namespace std;

void setNode(Node* n, Node* left=NULL, Node* right=NULL)
{
  n->left = left;
  n->right = right;
}

testing::AssertionResult checkEqualPathsResult(bool expected, Node* root, const char* tree)
{
    bool actual = equalPaths(root);
    if (expected == actual){
        return testing::AssertionSuccess();
    }
    else{
        return testing::AssertionFailure() << endl << boolalpha <<
        "[Values do not match] " << endl << "  Expected: " << expected << endl << "  Actual: " << actual << endl <<"Tree used: "<< endl << tree << endl;
    }
}

/**
 *  root => NULL
 *  expected = true
 */
TEST(EqualPaths,Empty)
{
    const char* t = " <empty=NULL> \n";
    bool expected = true;
    EXPECT_TRUE(checkEqualPathsResult(expected, nullptr, t));
}


/**
    root => a
    expected = true
*/
TEST(EqualPaths,RootOnly)
{
    Node a(1);
    const char* t = " a \n";
    bool expected = true;
    EXPECT_TRUE(checkEqualPathsResult(expected, &a, t));
}


/**
  root => a
         /  
        b
    expected = true
*/
TEST(EqualPaths,RootLeft)
{
    Node a(10);
    Node b(8);
    setNode(&a,&b,NULL);
    setNode(&b,NULL,NULL);
  const char* t =
    "     a \n"
    "    /   \n"
    "   b    \n";
    bool expected = true;
    EXPECT_TRUE(checkEqualPathsResult(expected, &a, t));
}


/**
  root => a
            \\  
               b
   expected = true
*/
TEST(EqualPaths,RootRight)
{
    Node a(8);
    Node b(10);
    setNode(&a,NULL,&b);
    setNode(&b,NULL,NULL);
  const char* t =
    "     a    \n"
    "      \\   \n"
    "        b \n";
    bool expected = true;
    EXPECT_TRUE(checkEqualPathsResult(expected, &a, t));
}


/**
  root => a
        /   \\  
      b       c
   expected = true
*/
TEST(EqualPaths,Full1)
{
    Node a(8);
    Node b(7);
    Node c(10);
    setNode(&a,&b,&c);
    setNode(&b,NULL,NULL);
    setNode(&c,NULL,NULL);
  const char* t =
    "     a     \n"
    "   /   \\   \n"
    "  b     c  \n";
    bool expected = true;
    EXPECT_TRUE(checkEqualPathsResult(expected, &a, t));
}

/**
    root => a    
          /  \\
         b     c
        /       \\
       d         e
   expected = true
 */
TEST(EqualPaths,TrueTest1)
{
    Node a(1);
    Node b(2);
    Node c(3);
    Node d(4);
    Node e(5);
    setNode(&a,&b,&c);
    setNode(&b,&d,NULL);
    setNode(&c,NULL,&e);
    setNode(&d,NULL,NULL);
    setNode(&e,NULL,NULL);
    const char* t =
    "      a        \n"
    "    /  \\       \n"
    "   b     c       \n"
    "  /       \\    \n"
    " d         e    \n"
    "";
    bool expected = true;
    EXPECT_TRUE(checkEqualPathsResult(expected, &a, t));
}

/**
    root => a    
          /  \\
         b     c
        / \     \\
       d   f      e
   expected = true
 */
TEST(EqualPaths,TrueTest2)
{
    Node a(1);
    Node b(2);
    Node c(3);
    Node d(4);
    Node e(5);
    Node f(6);
    setNode(&a,&b,&c);
    setNode(&b,&d,&f);
    setNode(&c,NULL,&e);
    setNode(&d,NULL,NULL);
    setNode(&e,NULL,NULL);
    setNode(&f,NULL,NULL);
    const char* t =
    "      a        \n"
    "    /  \\       \n"
    "   b     c       \n"
    "  / \\     \\    \n"
    " d   f      e    \n"
    "";
    bool expected = true;
    EXPECT_TRUE(checkEqualPathsResult(expected, &a, t));
}


/**
    root => a    
          /  \\
         b     c
        /    /   \\
       d    f     e
   expected = true
 */
TEST(EqualPaths,TrueTest3){
    Node a(1);
    Node b(2);
    Node c(3);
    Node d(4);
    Node e(5);
    Node f(6);
    setNode(&a,&b,&c);
    setNode(&b,&d,NULL);
    setNode(&c,&f,&e);
    setNode(&d,NULL,NULL);
    setNode(&e,NULL,NULL);
    setNode(&f,NULL,NULL);
    const char* t =
    "      a        \n"
    "    /  \\       \n"
    "   b     c       \n"
    "  /     /  \\    \n"
    " d     f    e    \n"
    "";
    bool expected = true;
    EXPECT_TRUE(checkEqualPathsResult(expected, &a, t));
}

/**
    root => a    
          /  \\
         b     c
        / \\   /   \\
       d   f  e   g
   expected = true
 */
TEST(EqualPaths,TrueTest4){
    Node a(1);
    Node b(2);
    Node c(3);
    Node d(4);
    Node e(5);
    Node f(6);
    Node g(7);
    setNode(&a,&b,&c);
    setNode(&b,&d,&f);
    setNode(&c,&e,&g);
    setNode(&d,NULL,NULL);
    setNode(&e,NULL,NULL);
    setNode(&f,NULL,NULL);
    setNode(&g,NULL,NULL);
    const char* t =
    "      a        \n"
    "    /  \\       \n"
    "   b     c       \n"
    "  / \\   /   \\    \n"
    " d   f  e   g    \n"
    "";
    bool expected = true;
    EXPECT_TRUE(checkEqualPathsResult(expected, &a, t));
}

/**
    root => a    
          /  \\
         b     c
        / \\   /   \\
       d   f  e   g
          /
         h
   expected = false
 */
TEST(EqualPaths,FalseTest1){
    Node a(1);
    Node b(2);
    Node c(3);
    Node d(4);
    Node e(5);
    Node f(6);
    Node g(7);
    Node h(8);
    setNode(&a,&b,&c);
    setNode(&b,&d,&f);
    setNode(&c,&e,&g);
    setNode(&d,NULL,NULL);
    setNode(&e,NULL,NULL);
    setNode(&f,&h,NULL);
    setNode(&g,NULL,NULL);
    setNode(&h,NULL,NULL);
    const char* t =
    "      a        \n"
    "    /  \\       \n"
    "   b     c       \n"
    "  / \\   /   \\    \n"
    " d   f  e   g    \n"
    "    /    \n"
    "   h    \n"
    "";
    bool expected = false;
    EXPECT_TRUE(checkEqualPathsResult(expected, &a, t));
}

/**
    root => a    
          /  \\
         b     c
             /   \\
            f     e
   expected = false
 */
TEST(EqualPaths,FalseTest2){
    Node a(1);
    Node b(2);
    Node c(3);
    Node e(5);
    Node f(6);
    setNode(&a,&b,&c);
    setNode(&b,NULL,NULL);
    setNode(&c,&f,&e);
    setNode(&e,NULL,NULL);
    setNode(&f,NULL,NULL);
    const char* t =
    "      a        \n"
    "    /  \\       \n"
    "   b     c       \n"
    "        /  \\    \n"
    "       f    e    \n"
    "";
    bool expected = false;
    EXPECT_TRUE(checkEqualPathsResult(expected, &a, t));
}

/**
    root => a    
             \\
               b
              / \\
             c   d
                / \\
               e   f
              / \\
             g   h
            /    
           i
  expected = false
 */
TEST(EqualPaths,FalseTest3){
    Node a(1);
    Node b(3);
    Node c(2);
    Node d(8);
    Node e(6);
    Node f(9);
    Node g(5);
    Node h(7);
    Node i(4);
    setNode(&a,NULL,&b);
    setNode(&b,&c,&d);
    setNode(&c,NULL,NULL);
    setNode(&d,&e,&f);
    setNode(&e,&g,&h);
    setNode(&f,NULL,NULL);
    setNode(&g,&i,NULL);
    setNode(&h,NULL,NULL);
    setNode(&i,NULL,NULL);
    const char* t =
    "  a        \n"
    "  \\       \n"
    "   b       \n"
    "   / \\    \n"
    "  c   d    \n"
    "     / \\  \n"
    "    e   f  \n"
    "   / \\    \n"
    "  g   h    \n"
    " /         \n"
    "i          \n"
    "";
    bool expected = false;
    EXPECT_TRUE(checkEqualPathsResult(expected, &a, t));
}




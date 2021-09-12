// include this file at most once 
#ifndef TEST_SPLAY_H
#define TEST_SPLAY_H


#include <cppunit/extensions/HelperMacros.h>
#include "splay.h"

class SplayTreeTest : public CPPUNIT_NS::TestFixture {

  // N.B. - all test methods have to be 
  // explicitly added to the test suite 
  // here by using the macro CPPUNIT_TEST() 
  // on each test method 
    
  CPPUNIT_TEST_SUITE(SplayTreeTest);
  CPPUNIT_TEST(testInsert);
  CPPUNIT_TEST(testFind);
  CPPUNIT_TEST(testRemoveAll);
  CPPUNIT_TEST(testRemoveOne);
  CPPUNIT_TEST_SUITE_END();

  public:
    void setUp();
    void tearDown();

    void testInsert();
    void testInsertHelper(int seed);
    void testFind();
    void testFindHelper(int seed);
    void testRemoveAll();
    void testRemoveAllHelper(int seed);
    void testRemoveOne();
    // void test2();

  private:
    // SplayTree object to test 
    SplayTree * tree;
};

// helper functions for testing splay tree
bool checkChildParentPtrs(STNode* node);
bool checkAllChildParentPtrs(STNode *root);



#endif 

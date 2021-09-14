#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <vector>

#include "test-utils.h"
#include "test-splay.h"
#include "splay.h"

using namespace std;
using vi = vector<int>;

void SplayTreeTest::setUp() {
  // cout << "setting up" << endl;
  tree = new SplayTree();
}

void SplayTreeTest::tearDown() {
  // cout << "tearing down" << endl;
  delete tree;
}

// TODO 
// - run tests with many different seeds 
//  -- generalize this 
// - add subtree sizes and test them 
//  -- could speed up subtree size test by writing it manually
//  -- - current version calls countNodes on every node, 
//     - when we could just run the test in a single call to countNodes. 
//      ( i.e., a single bottom-up traversal ) 
// - test balance (won't always be balanced, but should be most of the time) 

// test subtree sizes update with inserts only 
void SplayTreeTest::testSizesWithInsert() {
  int numNodes = 100;
  int seed = 1; 

  SubtreeSizePred sspred;
  vi ints = randomInts(numNodes, seed);
  for (int i : ints) {
    tree->insert(i);
    // test that all subtree size 
    // augmentations are correct
    CPPUNIT_ASSERT(sspred.testTree(*tree));
  }

  std::cout << "Subtree size w/ insert passed." << std::endl;
}

// test subtree sizes after removing some nodes 
void SplayTreeTest::testSizesWithRemove() {
  int numNodes = 100;
  int seed = 5; 

  vi ints = randomInts(numNodes, seed);
  for (int i : ints) {
    tree->insert(i);
  }

  // remove half of the nodes 
  for (int i = 0 ; i < numNodes / 2; i++) 
    tree->remove(ints[i]);

  SubtreeSizePred sspred;
  // test that all subtree size 
  // augmentations are correct
  CPPUNIT_ASSERT(sspred.testTree(*tree));

  std::cout << "Subtree size w/ remove passed." << std::endl;
}


// try inserting nodes using many different seeds
void SplayTreeTest::testInsert() {
  int numSeeds = 100;
  for (int s : randomInts(numSeeds)) {
    testInsertHelper(s);
    tearDown();
    setUp();
  }
}


// just try inserting a bunch of nodes 
void SplayTreeTest::testInsertHelper(int seed) {
  int numNodes = 100;
  vi ints = randomInts(numNodes, seed);
  for (int i : ints) {
    tree->insert(i);
    // test that inserted node becomes root 
    CPPUNIT_ASSERT(tree->root->key == i); 
  }

  // assert bst (ordering) invariant is satisfied for all nodes 
  BSTPred bstPred; 
  CPPUNIT_ASSERT(bstPred.testTree(*tree));

  // assert all child/parent pointers match 
  ChildParentPred childParentPred; 
  CPPUNIT_ASSERT(childParentPred.testTree(*tree));

  cout << "inserted " << numNodes << " nodes successfully." << endl;
}


void SplayTreeTest::testFind() {
  int numSeeds = 100;
  for (int s : randomInts(numSeeds)) {
    testFindHelper(s);
    tearDown();
    setUp();
  }
}

void SplayTreeTest::testFindHelper(int seed) {
  int numNodes = 100;
  vi ints = randomInts(numNodes, seed);
  for (int i : ints) {
    tree->insert(i);
    CPPUNIT_ASSERT(tree->find(i) != nullptr);
    CPPUNIT_ASSERT(tree->find(i)->key == i);
  }
}


// try inserting and removing a single node 
void SplayTreeTest::testRemoveOne() {
  tree->insert(55);
  tree->insert(59);
  tree->remove(55);
  SubtreeSizePred sspred;
  CPPUNIT_ASSERT(sspred.testTree(*tree));
}

// try inserting a bunch of nodes 
// and then deleting them all 
void SplayTreeTest::testRemoveAll() {
  int numSeeds = 100;
  for (int s : randomInts(numSeeds)) {
    testRemoveAllHelper(s);
    tearDown();
    setUp();
  }
}

void SplayTreeTest::testRemoveAllHelper(int seed) {
  int numNodes = 100;
  vi ints = randomInts(numNodes, seed, 10*numNodes);
  for (int i : ints) 
    tree->insert(i);

  SubtreeSizePred sspred;
  for (int i = 0; i < ints.size(); i++) {
    // key should be present before removal 
    STNode * f = tree->find(ints[i]);
    CPPUNIT_ASSERT(f != nullptr);
    CPPUNIT_ASSERT(f->key == ints[i]);

    // key should be absent post removal 
    tree->remove(ints[i]);
    f = tree->find(ints[i]);
    CPPUNIT_ASSERT(f == nullptr);

    // assert subtree sizes are satisfied 
    CPPUNIT_ASSERT(sspred.testTree(*tree));
  }

  std::cout << "removed all " << numNodes << " nodes successfully." << std::endl;
}

int main() {
  // N.B. - all test methods have to be 
  // explicitly added to the test suite 
  // in the test header file. 

  // register text fixture (SplayTreeTest) to registry. 
  // test fixture consists of a suite of related tests
  CPPUNIT_TEST_SUITE_REGISTRATION( SplayTreeTest );

  // Get the top level suite from the registry
  CppUnit::Test *suite = 
    CppUnit::TestFactoryRegistry::getRegistry().makeTest();

  CppUnit::TextUi::TestRunner runner;
  runner.addTest(suite);
  bool wasSucessful = runner.run();

  return wasSucessful ? 0 : 1;
}



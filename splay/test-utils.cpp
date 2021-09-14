#include <iostream>
#include <bits/stdc++.h>
#include <assert.h>
#include "test-utils.h"



// generate length n vector of random integers 
// with no repeats 
//
// TODO use a faster method of removing repeats 
// .. could just generate 1...maxVal, shuffle, and use first n.
std::vector<int> randomInts(int n, int seed, int maxVal) {
  maxVal = std::max(maxVal, n+1);
	assert(n < maxVal);

  std::vector<int> ints; 
  std::unordered_set<int> intset; 
  for (int i = 0; i < n; i++) {
    int x = rand() % maxVal;
    while (intset.find(x) != intset.end()) {
      x = rand() % maxVal;
		}
    
    intset.insert(x);
    ints.push_back(x);
  }
  intset.clear();
  return ints;
}


// count nodes in a subtree
// for testing augmented sizes 
int countNodes(STNode *node) {
  if (node == nullptr) return 0;
  return 1 + countNodes(node->left) + countNodes(node->right);
}


// check all nodes using 
// NodePredicate pointer provided in constructor 
bool TreePredicate::testTree(SplayTree &t) {
  return allNodesSatisfy(nodeProp, t);
}


bool ChildParentPredicate::testNode(STNode *node) {
  // null node trivially satisfies predicate
  if (node == nullptr) return true; 

  // check node->parent and node 
  bool parentOk = true;
  if (node->hasParent()) {
    if (node->isLeftChild())
      parentOk &= node->parent->left == node;
    if (node->isRightChild())
      parentOk &= node->parent->right == node;
  }

  // check children
  bool leftOk, rightOk;
  leftOk = rightOk = true;
  if (node->hasLeftChild())
    leftOk &= node->left->parent == node;
  if (node->hasRightChild())
    rightOk &= node->right->parent == node;

  return parentOk && leftOk && rightOk;
}



// check whether left subchild is < node
// and right subchild is > node 
bool BSTNodePredicate::testNode(STNode *node) {
  bool leftOK, rightOK; 
  leftOK = rightOK = true;

  if (node->hasLeftChild())
    leftOK &= node->left->key < node->key;
  if (node->hasRightChild())
    rightOK &= node->right->key > node->key;
  return leftOK && rightOK;
}

// node satisfies this predicate if its augmented 
// sizes matches its actual (manually computed) size 
bool SubtreeSizePredicate::testNode(STNode *node) {
  return node->size == countNodes(node);
}

// test whether all nodes in a tree satisfy a predicate 
bool allNodesSatisfy(NodePredicate *pr, SplayTree &t) {
  assert(pr != nullptr);
  return _allNodesSatisfy(pr, t.root);
}

// test whether all nodes in a subtree rooted at node 
// satisfy a predicate 
bool _allNodesSatisfy(NodePredicate *pr, STNode *node) {
  // empty tree trivially satisfies 
  if (node == nullptr) return true; 

  // do all nodes satisfy constraint
  return pr->testNode(node)
    &&   _allNodesSatisfy(pr, node->left)
    &&   _allNodesSatisfy(pr, node->right);
}




// // check whether all nodes in tree 
// // have matching child/parent pointers 
// bool checkAllChildParentPtrs(STNode* root) {
//   // empty tree trivially satisfies 
//   if (root == nullptr) return true; 
// 
//   std::cout << "checking " << root->key << std::endl;
// 
//   // bool rt = checkChildParentPtrs(root);
//   // assert(rt);
//   // bool left = checkAllChildParentPtrs(root->left);
//   // bool right = checkAllChildParentPtrs(root->right);
//   // return rt && left && right;
// 
//   // do all nodes satisfy constraint
//   return checkChildParentPtrs(root)
//     &&   checkAllChildParentPtrs(root->left)
//     &&   checkAllChildParentPtrs(root->right);
// }
// 


// // check that parent pointers match 
// // child pointers for a node 
// //
// // return true if things match
// bool checkChildParentPtrs(STNode* node) {
//   // null node trivially satisfies predicate
//   if (node == nullptr) return true; 
// 
//   // check node->parent and node 
//   bool parentOk = true;
//   if (node->hasParent()) {
//     if (node->isLeftChild())
//       parentOk &= node->parent->left == node;
//     if (node->isRightChild())
//       parentOk &= node->parent->right == node;
//   }
// 
//   // check children
//   bool leftOk, rightOk;
//   leftOk = rightOk = true;
//   if (node->hasLeftChild())
//     leftOk &= node->left->parent == node;
//   if (node->hasRightChild())
//     rightOk &= node->right->parent == node;
// 
//   return parentOk && leftOk && rightOk;
// }
// 

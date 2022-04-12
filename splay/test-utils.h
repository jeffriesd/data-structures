#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <vector>
#include "splay.h"


const int MAX_VAL = 10000;
// generate n random non-negative integers upper bounded by maxVal
std::vector<int> randomInts(int n, int seed=0, int maxVal=MAX_VAL);

// count nodes in a subtree
int countNodes(STNode * node);

// compute hash of inorder traversal of 
// subtree rooted at this node 
ll hashInorder(STNode * node);

// TODO add traceback for when test fails on a particular node 

// to create a new predicate for a tree: 
// - declare derived class of TreePredicate
// - define its constructor (choose a node predicate)
//   in this header file 
//
// to create a new predicate for a node:
// - declare derived class of NodePredicate
// - define its testNode method in .cpp file 

// NodePredicate is used to encapsulate 
// a predicate on a splay tree node. 
// To define a new predicate, inherit 
// from NodePredicate and define testNode. 
//
// We can then define a function that takes a 
// NodePredicate pointer and checks whether the 
// predicate holds for all nodes. This way we only 
// need a single "allNodesSatisfyX" function
// instead of copying the tree traversal boilerplate 
// in a new method for each node predicate. 
class NodePredicate {
  public: 
    // pure virtual function 
    virtual bool testNode(STNode *node) = 0;
};

// superclass for tests on trees 
//
// TreePredicate encapsulates the action of 
// checking a NodePredicate against all nodes in a splay tree. 
// It is constructed with a single NodePredicate pointer which 
// determines which predicate is checked for each node. 
// It provides a single public method which checks whether
// all nodes satisfy the provided predicate. 
class TreePredicate {
  private: 
    NodePredicate * nodeProp;

  protected:
    TreePredicate(NodePredicate * np) : nodeProp(np) { }
    ~TreePredicate() { delete nodeProp; }
    // TreePredicate() { setNodeProp(); }
    
  public:
    // check predicate on all nodes in a tree
    bool testTree(SplayTree &t);
};


/********************************************************
* NODE PREDICATES 
********************************************************/

// checks whether left child key is < 
// and right child key is > current node key
//
class BSTNodePredicate : public NodePredicate {
  public:
    bool testNode(STNode * node);
};

class ChildParentPredicate : public NodePredicate { 
  public:
    bool testNode(STNode * node);
};

// node satisfies this predicate if its augmented 
// sizes matches its actual (manually computed) size 
class SubtreeSizePredicate : public NodePredicate { 
  public:
    bool testNode(STNode * node);
};

// node satisfies this predicate if its augmented
// hash value matches the polynomial hash
// of the inorder traversal of the subtree
// rooted at this node  
class SubtreeHashPredicate : public NodePredicate { 
  public:
    bool testNode(STNode * node);
};



/********************************************************
* TREE PREDICATES (end in "...Pred")
********************************************************/

class BSTPred : public TreePredicate {
  public:
    BSTPred() : TreePredicate(new BSTNodePredicate()) {}
};

class ChildParentPred : public TreePredicate { 
  public:
    ChildParentPred() : TreePredicate(new ChildParentPredicate()) {}
};

class SubtreeSizePred : public TreePredicate { 
  public:
    SubtreeSizePred() : TreePredicate(new SubtreeSizePredicate()) {}
};

class SubtreeHashPred : public TreePredicate { 
  public:
    SubtreeHashPred() : TreePredicate(new SubtreeHashPredicate()) {}
};



// test (any) node predicate on entire tree 
bool allNodesSatisfy(NodePredicate *pr, SplayTree &t);
bool _allNodesSatisfy(NodePredicate *pr, STNode * node);


#endif 

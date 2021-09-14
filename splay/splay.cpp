#include "splay.h"
#include "test-utils.h" // just for debugging
#include <assert.h>
#include <iostream>
#include <string>


// TODO augment nodes with subtree sizes 
// - when do they need to be updated? 
//   - insertion 
//   - removal 
//   - rotations 
//   - replaceNode, etc. 


// TODO 
// - implement upperBound(), lowerBound() 
// - multiset 
// - range counting 
// - rank, select 

// destructor for splay tree 
// - delete all dynamically-allocated nodes 
//   in the tree by deleting the root 
SplayTree::~SplayTree() {
  delete root;
}

// delete left and right subtree pointers
//
// no need to delete parent pointers 
// because each node deletes its children.
// deleting in both directions would be redundant. 
STNode::~STNode() {
  delete left; 
  delete right; 
}

// set size to lsize + 1 + rsize 
void STNode::updateSizeFromChildren() {
  int subtreeSize = 1; 
  if (hasLeftChild())
    subtreeSize += left->size;
  if (hasRightChild())
    subtreeSize += right->size;
  size = subtreeSize;
}

// update all subtree sizes 
// from this node to root 
void STNode::updateSizeToRoot() {
  STNode * cur;
  for (cur = this; cur != nullptr; cur = cur->parent)
    cur->updateSizeFromChildren();
}

// set left child and update parent pointer for child  
void STNode::setLeftChild(STNode * n) {
  left = n;
  if (n != nullptr)
    n->parent = this;
}

// set right child and update parent pointer for child  
void STNode::setRightChild(STNode * n) {
  right = n;
  if (n != nullptr)
    n->parent = this;
}


// print keys of parent, this node, left child, and right child
// for debugging. 
void STNode::printNeighbors() {
  if (this == nullptr) return; 
  
  std::string ps = "NULL"; 
  std::string ls = "NULL"; 
  std::string rs = "NULL"; 
  if (parent != nullptr)
    ps = std::to_string(parent->key);
  if (left != nullptr)
    ls = std::to_string(left->key);
  if (right != nullptr)
    rs = std::to_string(right->key);

  std::cout << "p:" << ps << std::endl;
  std::cout << "->" << key << std::endl;
  std::cout << "\t-left->" << ls << std::endl;
  std::cout << "\t-right->" << rs << std::endl;
}

bool STNode::hasParent() {
  return parent != nullptr;
}

bool STNode::hasGrandP() {
  if (! hasParent())
    return false;
  return parent->hasParent();
}

bool STNode::hasLeftChild() {
  return left != nullptr;
}

bool STNode::hasRightChild() {
  return right != nullptr;
}

/**
 * Rotating a node x with its parent
 *
 *      gp            gp
 *      p             x
 *     / \           / \
 *    x   c  ---->  a   p
 *   / \               / \
 *  a   b             b   c
 *
 *  <--- rotate left
 *  ---> rotate right 
 *
 *  subtree sizes: 
 *  gp, a, b, c, sizes unchanged 
 *
 *  to update p and x: 
 *  update p and then update x 
 *  since p becomes child of x after rotation
 */
void STNode::rotate() {
  // nothing to do for root node
  if (! hasParent()) return;

  // original parent, grandparent 
  STNode * p = parent;
  STNode* gp = parent->parent;
  
  if (isLeftChild()) 
    rotateRight();
  else if (isRightChild()) 
    rotateLeft();

  // update grandparent references 
  if (gp != nullptr) {
    if (gp->left == p) 
      gp->setLeftChild(this);
    else if (gp->right == p)
      gp->setRightChild(this);
  }
  else // this node has become root 
    parent = gp;

  // update subtree sizes.
  // update p first
  p->updateSizeFromChildren();

  // then update current node (called x above)
  updateSizeFromChildren();
}

/*
 *      gp            gp
 *      p             x
 *     / \           / \
 *    a   x  ---->  p   c
 *       / \       / \
 *      b   c     a   b    
 */
// right children should rotate left
// - rotate left and update subtree sizes 
//
// subtree sizes:
// a, b, c, gp unchanged
//
// to update p: 
// p->size = a->size + 1 + b->size
//
// to update x:
// x->size = p->size + 1 + c->size
void STNode::rotateLeft() {
  if (! isRightChild()) return;
  STNode *b, *p, *gp;
  b = left;
  p = parent;

  p->setRightChild(b); 
  setLeftChild(p);
}

// left children should rotate right 
void STNode::rotateRight() {
  if (! isLeftChild()) return;
  STNode *b, *p, *gp;
  b = right;
  p = parent;

  p->setLeftChild(b); 
  setRightChild(p);
}

bool STNode::isLeftChild() {
  if (! hasParent()) return false;
  return this == this->parent->left;
}

bool STNode::isRightChild() {
  if (! hasParent()) return false;
  return this == this->parent->right;
}

// check whether node has grandparent 
// and whether node and parent are both
// left/left or right/right children
bool STNode::zigZig() {
  return (isLeftChild()  && parent->isLeftChild())
      || (isRightChild() && parent->isRightChild());
}

// check whether node has grandparent 
// and whether node and parent are 
// left/right or right/left
bool STNode::zigZag() {
  return (isLeftChild()  && parent->isRightChild())
      || (isRightChild() && parent->isLeftChild());
}


// splay until node is root 
//
// (no grandp) 
// if node has parent but no grandparent, 
// rotate at node->parent
//
// (zigzig)
//
//     g
//      \
//       p
//        \
//         n
//
// if node has grandp and node
// and node->parent and 
// node and node->parent are both 
// left/right children, 
// rotate at node->parent->parent
// then 
// rotate at node->parent
// 
// (zigzag)
//
//     g
//      \
//       p
//      /
//     n
//
// if node has grandp and 
// parent is left and node is right 
// or vice versa, then 
// rotate at node->parent
// then 
// rotate at the old grandparent 
void SplayTree::splay(STNode *node) {
  if (node == nullptr) { 
    assert(false);
    return;
  }
  // splay until cur becomes root 
  STNode * cur = node;
  while (cur->hasParent()) {
    // no grandparent 
    if (! cur->hasGrandP())
      cur->rotate();
    else {
      // cur has grandparent and therefore 
      // cur has a parent 
      STNode * p = cur->parent;
      // left/left or right/right
      if (cur->zigZig()) {
        p->rotate();
        cur->rotate();
      }
      // left/right or right/left 
      else if (cur->zigZag()) {
        cur->rotate();
        cur->rotate();
      }
    }
  }

  // update root 
  root = node;
}

// insert new node with key k in splay tree 
void SplayTree::insert(int k) {
  // try finding first (without splaying)
  STNode * n = _find(root, k);
  if (n != nullptr) {
    // TODO handle multiple of same key.. just use count?
    assert(false);
  }

  root = _insert(root, k);
  root->parent = nullptr;

  assert(insertedNodePtr != nullptr);
  // splay after inserting 
  splay(insertedNodePtr);
  insertedNodePtr = nullptr;
  assert(root != nullptr);


  // alternatively, we could get rid of 
  // insertedNodePtr and just call find() on 
  // inserted key after _insert. 
  // but insertedNodePtr is faster because it 
  // doesn't require an additional O(log n) call 
}


bool STNode::isLeaf() {
  return ! (hasLeftChild() || hasRightChild());
}

// replace node n with node m 
// - m may be null
// - but n cannot be null 
//
// n is unaffected. its parent/children 
// are updated to connect to m. 
//
// children of n/m are unaffected 
//
// can be used to delete a subtree by using m = nullptr
void SplayTree::replaceNode(STNode * n, STNode * m) {
  assert(n != nullptr);

  // check if n is root 
  if (root == n)
    root = m;

  // update child pointers for parent of n
  if (n->isLeftChild())
    n->parent->setLeftChild(m);
  else if (n->isRightChild())
    n->parent->setRightChild(m);

  // update parent pointer of m.
  //
  // need to do this in case neither 
  // isLeftChild() nor isRightChild() are true 
  if (m != nullptr)
    m->parent = n->parent;
}

// remove a particular node 
void SplayTree::removeNode(STNode * node) {
  assert(node != nullptr);

  STNode * nodeToSplay = node->parent;

  if (! node->hasLeftChild()) {
    // delete node by turning edges 
    // from node's parent to node's child 
    // into a single edge
    // so parent--node--child becomes
    // parent--child
    //
    // N.B. child can be null 
    replaceNode(node, node->right); 

    // update subtree size of node parent 
    if (node->hasParent())
      node->parent->updateSizeFromChildren();
  } else if (! node->hasRightChild()) {
    replaceNode(node, node->left);
    // update subtree size of node parent 
    if (node->hasParent())
      node->parent->updateSizeFromChildren();
  }
  else {
    // swap with predecessor (maximumLeaf of left subtree)
    STNode * pred = node->left->maximumLeaf();

    // special case where pred is left child of node 
    if (pred->parent == node) {
      // splay pred instead of pred->parent 
      // since its parent is node itself
      nodeToSplay = pred;

      // put pred in node's position
      replaceNode(node, pred);

      // connect pred to right subtree of node
      pred->setRightChild(node->right);

      // update subtree sizes from pred to root
      pred->updateSizeToRoot();
    }
    // if there are some nodes between node and pred
    else {
      STNode * originalPredParent = pred->parent;
      STNode * originalPredChild = pred->left;

      // splay parent node of pred (see Tarjan book)
      nodeToSplay = originalPredParent;

      // need to put pred in node's position
      // and then update pred's children to be 
      // node's children 
      replaceNode(node, pred);

      // set left child of pred to left child of node
      pred->setLeftChild(node->left);
      pred->setRightChild(node->right);
    
      // then remove the original pred. 
      // N.B., pred is right child by definition
      originalPredParent->setRightChild(originalPredChild);

      // need to update subtree sizes 
      // from originalPredParent to root
      originalPredParent->updateSizeToRoot();
    }
  }


  // splay the STNode that was the parent of node immediately before deletion
  if (nodeToSplay != nullptr)
    splay(nodeToSplay);
}

// remove a node with key k 
void SplayTree::remove(int k) {
  // find without splaying 
  STNode * node = _find(root, k);
  if (node == nullptr) return; 

  removeNode(node);

  // deallocate memory for removed node 
  //
  // N.B. we have to set its left/right ptrs 
  // to null first, otherwise an entire subtree is deleted
  node->left = nullptr; 
  node->right = nullptr;
  delete node;
}

// swap two nodes, where first argument 
// is possibly the root 
void SplayTree::swapNodeValues(STNode * n, STNode * m) {
  // // if second argument is root, change order of arguments
  // if (m == root) { 
  //   swapNodes(m, n); 
  // }
  // assert(m != root);
  assert(n != nullptr && m != nullptr);

  int t = n->key;
  n->key = m->key;
  m->key = t;
}

// find rightmost node in subtree
STNode * STNode::maximumLeaf() {
  if (hasRightChild())
    return right->maximumLeaf();
  return this;
}

// find leftmost node in subtree
STNode * STNode::minimumLeaf() {
  if (hasLeftChild())
    return left->minimumLeaf();
  return this;
}

// insert new node with key k in 
// subtree rooted at node and
// return a pointer to the root
// of the subtree after insertion 
//
// precondition: 
// - int k is not already present in 
//   subtree rooted at node 
STNode* SplayTree::_insert(STNode* node, int k) {
  if (node == nullptr) { 
    STNode * newNode = new STNode(k); 
    // set insertedNodePtr so new node 
    // can be splayed after insertion 
    insertedNodePtr = newNode;
    return newNode;
  }

  if (k < node->key) 
    node->setLeftChild(_insert(node->left, k));
  else if (k > node->key) 
    node->setRightChild(_insert(node->right, k));

  // update subtree size 
  node->updateSizeFromChildren();

  return node;
}



// find node with key k in splay tree 
STNode * SplayTree::find(int k) {
  STNode* n = _find(root, k);

  // splay after accessing
  if (n != nullptr) 
    splay(n);

  return n;
}

// find key k in subtree rooted at node 
STNode * SplayTree::_find(STNode* node, int k) {
  if (! node) return nullptr;  

  // print address 
  // std::cout << node << std::endl;

  if (k == node->key) return node;
  if (k < node->key)
    return _find(node->left, k);
  return _find(node->right, k);
}


void SplayTree::printInorder() {
  _printInorder(root);
  std::cout << std::endl;
}

void SplayTree::_printInorder(STNode* node) {
  if (node == nullptr) return;
  _printInorder(node->left);
  std::cout << node->key << " ";
  _printInorder(node->right);
}

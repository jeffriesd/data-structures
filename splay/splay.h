#ifndef SPLAY_H
#define SPLAY_H

#include<vector>

// splay tree invariants: 
// - at most one of each key 

typedef unsigned long long ll;

// prime for hashing 
//
// this prime is large enough 
// to avoid collisions 
const ll P = 104729;

// mod by M in hash
const ll M = 1UL<<63;

// binary exponentiation
ll binpow(ll a, ll b);

class STNode {

  public:
    STNode * left;
    STNode * right;
    STNode * parent;
    int key;

    // hash of this subtree
    //
    // it is computed as a 
    // polynomial hash of the 
    // inorder traversal of the 
    // subtree rooted at this node
    //
    // hash(x1,x2,..,xn)
    // = 
    // (x1 + x2*p + ... + xn*p^(n-1)) % m 
    //
    // need to choose large m for fewer collisions.
    //
    // according to cp-algorithms, 
    // p should be roughly equal to 
    // the size of the input alphabet
    // i.e. the range of (x1,...,xn).  
    //
    // not sure what p should be in general.. 
    // could parameterize SplayTree with p. 
    ll hash;

    // subtree size
    int size; 

    STNode(int k) 
      : left(nullptr), 
        right(nullptr), 
        parent(nullptr),
        size(1),
        key(k), 
        hash(k % M) { }

    ~STNode();

    bool isLeftChild()   const;
    bool isRightChild()  const;
    bool hasLeftChild()  const;
    bool hasRightChild() const;
    bool hasParent()     const;
    bool hasGrandP()     const;
    bool isLeaf()        const;

    void setLeftChild(STNode * n);
    void setRightChild(STNode * n);

    // check shape of subtree for splaying 
    bool zigZig();
    bool zigZag();

    // rotate node with parent 
    void rotate();
    void rotateLeft();
    void rotateRight();

    // set size to lsize + 1 + rsize 
    void updateSizeFromChildren();

    // update polynomial hash for this node
    // based on children and subtree sizes
    void updateHashFromChildren();

    // update all augmentations that are
    // affected by rotations
    //
    // TODO: does hash need to be updated 
    // iff size needs to be updated? 
    //
    // if size needs to be updated: 
    // - node is added or removed in subtree,
    // so hash needs to be updated
    //
    // if hash needs to be updated:
    // - either current node key changes
    // or a key changes in a subtree. 
    // but we don't allow in-place key change, 
    // so this should imply size needs to be updated
    void updateAugmentations();

    // store inorder traversal in vector
    void getInorder(std::vector<int> &v) const;

    // update subtree augmentations (sizes, etc.) 
    // on path from this node to root 
    void updateAugToRoot();

    // maximumLeaf value node in this subtree 
    STNode * maximumLeaf();
    // min value node in this subtree 
    STNode * minimumLeaf();

    // testing/debugging function 
    // for printing parent, node, left child, right child
    void printNeighbors();
};

class SplayTree {

  private:
    // keep track of newly inserted leaf
    // node so it can be splayed after insertion 
    STNode * insertedNodePtr; 

    void splay(STNode *node);
    STNode * _find(STNode* n, int key);
    STNode * _insert(STNode* n, int key);
    void removeNode(STNode * node);

    void _printInorder(STNode *node);

  public:
    STNode * root; 
    STNode * find(int key);
    void insert(int key);
    void remove(int key);
    void swapNodeValues(STNode * n, STNode * m);

    // replace node n with node m 
    void replaceNode(STNode * n, STNode * m);

    SplayTree() : root(nullptr) { }
    ~SplayTree();

    void printInorder();

    void getInorder(std::vector<int> &v) const;
    int getSize() const;
    ll getHash()  const;
};

// compare based on hash
bool operator== (const SplayTree& t1, const SplayTree& t2);

void _getInorder(const SplayTree& t, std::vector<int> &v);
void _getInorder(const STNode& t, std::vector<int> &v);
#endif

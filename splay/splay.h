#ifndef SPLAY_H
#define SPLAY_H

// splay tree invariants: 
// - at most one of each key 


class STNode {

  public:
    STNode * left;
    STNode * right;
    STNode * parent;
    int key;

    // subtree size
    int size; 

    STNode(int k) 
      : left(nullptr), 
        right(nullptr), 
        parent(nullptr),
        size(1),
        key(k) { }

    ~STNode();

    bool isLeftChild();
    bool isRightChild();
    bool hasLeftChild();
    bool hasRightChild();
    bool hasParent();
    bool hasGrandP();
    bool isLeaf();

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

    // update subtree sizes on path 
    // from this node to root 
    void updateSizeToRoot();

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
};

#endif

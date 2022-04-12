# data-structures
Some tested C++ implementations of useful data structures.


## Splay trees
The `splay/` directory contains a tested C++ implementation of splay trees based on Tarjan's 
description of them in [Data Structures and Network Algorithms](https://epubs.siam.org/doi/book/10.1137/1.9781611970265?mobileUi=0&). 

To run the unit tests, run `make test`. To check for potential memory leaks from the unit tests, run `make memcheck` (or `make vmemcheck` for a verbose version). 

### Upcoming features
* multiset functionality 
* augment with subtree sizes - now added - updated lazily during rotations
* augment with subtree hash (polynomial hash of inorder traversal) - now added - hash for each node is lazily updated during rotations
* range queries 


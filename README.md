# Racenion 🏆

An advanced C++ tournament and team management system developed for the Data Structures course at the Technion – Israel Institute of Technology.

## Project Overview
Racenion is a real-time event tracking system that manages competing teams, individual contestants, matrix skill transformations, motivation scores, and team recruitment dynamics. It supports team registration, head-to-head duels, contestant mission count queries, experience updates, order-statistic collective motivation searches, and team acquisitions under tight time and space complexity bounds.

## Technical Highlights
* **Data Structures:** 
  * **Custom Hash Table:** Separate chaining with dynamic rehashing for expected $O(1)$ contestant lookups.
  * **Augmented AVL Trees:** Order-statistic rank trees maintaining subtree weights for logarithmic $i$-th element queries.
  * **Upgraded Union-Find (Disjoint Sets):** Implemented with Union by Size and Path Compression, supporting relative matrix multiplication and path propagation for mission and skill history tracking.
* **Memory Management:** Complete manual dynamic memory management without STL containers, ensuring exception safety and dynamic deallocation.

## File Structure
* `HashTable.h` — Custom template-based chaining hash table with automatic rehashing.
* `AVLTree.h` & `TreeNode.h` — Augmented AVL tree with subtree weight tracking (`weight`) for rank operations.
* `UnionFind.h` — Disjoint set structure handling path compression with relative weights.
* `Team.h`, `ContestantNode.h` — Core entities storing team and contestant metadata.
* `Racenion26b2.h` / `.cpp` — System interface and business logic implementations.
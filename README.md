# Threaded Red-Black Tree

This project implements a **threaded binary search tree** with **red-black balancing** and supports **range queries**.

Unlike a regular binary search tree, this structure is **threaded**:  
missing child pointers are not null dead ends. Instead, they store links to the next node in inorder traversal.  
This allows the tree to move through elements in sorted order without recursion or an auxiliary stack.

The custom tree supports two operations:

- **Insert** — inserts a key into the tree
- **RangeQuery(min, max)** — counts how many keys belong to the range

The project also includes a reference implementation based on **`std::set`** for comparison, as well as separate benchmarking executables.

## Prerequisites

- C++ compiler with modern C++ support
- **CMake 3.14+**

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Usage

- Benchmarks
```bash
./build/bench_tree
./build/bench_std_tree
```

- Main executables
```bash
./build/tree
./build/std_tree
```

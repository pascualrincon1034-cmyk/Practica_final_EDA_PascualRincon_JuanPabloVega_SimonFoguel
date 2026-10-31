# Sorting a Large Dataset in C++: QuickSort vs. HeapSort vs. AVL Tree

Final practice for **ST0245 – Data Structures and Algorithms**
School of Applied Sciences and Engineering, EAFIT University

**Authors:** Pascual Rincón Cardona · Simón Foguel Giraldo · Juan Pablo Vega Ruiz
**Lecturer:** Carlos Alberto Álvarez

---

## Overview

This project sorts a dataset of **100,000 English words** (randomly ordered, no duplicates) using three strategies built on different data structures, and compares them in terms of execution time, memory usage and algorithmic complexity.

| Strategy | Data structure | Core idea |
|---|---|---|
| **QuickSort** | `std::vector<std::string>` | In-place sorting with Lomuto partition (last element as pivot) |
| **HeapSort** | Binary max-heap over a `std::vector` | Heap built with Floyd's algorithm (`heapify`), then repeated root extraction |
| **AVL tree** | Self-balancing binary search tree | Insert all words, then inorder traversal into a vector |

All algorithms are implemented **from scratch**. No `std::sort`, `std::priority_queue`, `std::set` or `std::map` is used for the sorting logic.

## Repository structure

> Adjust the names below to match your repository.

```
.
├── src/
│   ├── main.cpp          # Loads the dataset, runs and times the three algorithms
│   ├── quicksort.hpp     # QuickSort (Lomuto partition)
│   ├── heapsort.hpp      # Binary heap + HeapSort
│   └── avl.hpp           # AVL tree (insertion, rotations, inorder traversal)
├── data/
│   └── dataset.txt       # 100,000 words, one per line (provided by the lecturer)
├── report/
│   └── report.pdf        # Final report
└── README.md
```

## Requirements

- A C++17 compiler (`g++` 9+ or `clang++` 10+ recommended)
- `make` (optional)

## Build and run

```bash
# Compile with optimizations (timings change a lot without -O2)
g++ -std=c++17 -O2 src/main.cpp -o sorting

# Run, passing the dataset path
./sorting data/dataset.txt
```

> If your program takes no arguments or uses a different path, update the commands above.

### What the program does

1. Loads all words from `dataset.txt` into a `std::vector<std::string>` (file I/O is **not** timed).
2. Runs each algorithm on an identical copy of the data.
3. Verifies that each output is sorted alphabetically.
4. Repeats the measurement 5 times and prints the mean and standard deviation.

Timing uses `std::chrono`. For the AVL tree, the timed region includes the insertions **and** the inorder traversal that produces the final sorted vector.

## Dataset

- Derived from `words_alpha.txt` (English words, one per line).
- Provided by the lecturer already shuffled, with 100,000 words.
- No repeated words, so the AVL tree stores no occurrence counts.

## Results

Execution time in **microseconds (µs)**, 5 runs per algorithm.

| Algorithm | Run 1 | Run 2 | Run 3 | Run 4 | Run 5 | Mean | Std. Dev. |
|---|---:|---:|---:|---:|---:|---:|---:|
| QuickSort | 54,228 | 53,469 | 51,128 | 50,496 | 49,976 | 51,859.4 | 1,880.30 |
| HeapSort | 108,803 | 101,803 | 100,392 | 104,787 | 105,826 | 104,322 | 3,329.71 |
| AVL tree | 152,598 | 157,455 | 139,840 | 137,374 | 144,095 | 146,272 | 8,520.01 |

QuickSort was the fastest: about **2.0× faster than HeapSort** and **2.8× faster than the AVL tree**.

> **Test machine:** _CPU, RAM, OS, compiler and version, flags (e.g. `-O2`). Fill this in._

### Algorithmic complexity

| Algorithm | Best | Average | Worst | Auxiliary space |
|---|---|---|---|---|
| QuickSort | O(n log n) | O(n log n) | O(n²) | O(log n) |
| HeapSort | O(n log n) | O(n log n) | O(n log n) | O(1) |
| AVL tree | O(n log n) | O(n log n) | O(n log n) | O(n) |

String comparisons are treated as O(1), since word length is bounded by the longest word in the dataset (the real cost is O(L)).

### Memory usage (estimates)

| Algorithm | Approximate memory | Notes |
|---|---|---|
| QuickSort | ~3.2 MB | Vector of 100,000 strings; O(log n) recursion stack |
| HeapSort | ~3.2 MB | Same vector, sorted in place; no extra memory |
| AVL tree | ~5.6 MB for nodes | 100,000 nodes with two pointers each, plus the iterative traversal stack and the output vector |

These are estimates based on `sizeof`. Real usage is higher because of padding, allocator overhead and strings longer than 15 characters (beyond small string optimization).

## Key takeaways

- **QuickSort** wins on this input: contiguous memory, sequential access and few operations per element. Its weakness is the O(n²) worst case with a fixed pivot, which does not appear here because the data is random and has no duplicates.
- **HeapSort** has guaranteed O(n log n) and needs no extra memory, but its access pattern jumps between distant positions in the array and it does more comparisons.
- **AVL tree** has the same asymptotic complexity but the highest constant cost: one dynamic allocation per node, pointer chasing, scattered memory (poor cache locality) and rebalancing work. It is better suited to scenarios with continuous insertions where order must be maintained.

The full analysis is in the [report](report/report.pdf).
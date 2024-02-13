# TiledKernel
## Introduction
TiledKernel is a **code generation library** based on **macro kernels** and **memory hierarchy graph data structure**.

TiledKernel aims to build a graph data structure and a series of macro kernels for code generation. The goal of TiledKernel is to generate high-performance fused kernels by leveraging a data structure that takes into account hardware characteristics and manually optimized macro kernels, while reducing scheduling overhead.

In most of the current state-of-the-art (SOTA) methods, hardware vendor-provided libraries or manually optimized CUDA C methods are commonly used for code generation. However, the implementation of library functions is often black-box and cannot be fused together. The scheduling space of CUDA C implementations is large, and their performance deviates significantly from library functions. Therefore, TiledKernel is dedicated to proposing an approach that balances performance and flexibility.

To achieve these goals, TiledKernel will provide a data structure based on graph memory hierarchy and performance-optimized macro kernels.

### Memory Hierarchy Graph Data Structure
To represent the entire computational process, **a data structure (I think we can just refer to it as the IR)** is required. While the specifics of the data structure can be set aside for now, the overall structure should be hierarchical in nature as shown below.

In this hierarchical dataflow graph, the dataflow edges have explicit operational semantics for data access.

- The in-edges represent read operations, while the out-edges represent write operations.
- There maybe various different implementations for these operations. For example, read operations can be translated into gather, slice, index, and other operations, while write operations can be translated into scatter, store, and so on. 

![](docs/figures/dataflow_graph.png)

![Register-level GEMM TileGraph description](docs/figures/RF_GEMM_Graph.png)

![](docs/figures/Shared_GEMM_Graph.png)

This data structure is named **TiledGraph** in this project. TiledGraph consists of **TiledEdge** and **TiledNode**.

**TiledEdge** is a directed edge that represents the **load** and **store** of data.

**TiledNode** is a node representation in TiledGraph, and it has multiple structures:
- Operation: Stateless primitive tensor operations.
- Task: A DAG build out of Operation Nodes and/or Task Nodes.
- Buffer: An addressable aggregation of statically shaped homogeneous tensors serving as a shared
memory among Task Nodes to merge the flow of control.

### Macro Kernels
To balance performance and usability, Macro Kernels are planned to be implemented using **cute**.

Additionally, to support computations at different levels, we also plan to enable the synthesis of multiple macro kernels into a micro_op and reuse macro kernels at higher memory hierarchy levels.

To avoid overly complex considerations, we take the example of Back2Back GEMM at the RF memory level. The dataflow graph can be represented simply as follows:

```
-----------         -----------      ---------------
| Load a'   |   --> | Load b'  |  -> | mma (a', b') |
-----------         -----------       ---------------
```

We will name the above dataflow graph structure as micro_op_a, representing a macro op at the RF level. Simultaneously, we can invoke micro_op_a as a macro op at the Shared Memory level, at the same level as Load/Store/Compute, and compose a higher-level dataflow graph. It is worth noting that when generating macro_ops at different levels, it is necessary to insert synchronization primitives at different levels.


As a motivating example, a back-to-back pseudocode can be represented as follows:

```
// inputs are on global memory, each dimension is iterated (controlled) by a loop
ASS: [M/TM][K/TK][TM, TK]
BSS: [N/TN][K/TK][TK, TN]
CSS: [P/TP][N/TN][TN, TP]
DSS: [M/TM][P/TP][TM, TP]  // output 

//  ============== Kernel Launch Semantics =============
// outers loops are mapped to kernel-launch semantics
for as in ASS:  // Block-mapped, --> block.x
  for cs in CSS:  // Block-mapped, --> block.y

    // ============  Inside a kernel function  ==============
    // allocate shared memory buffer for inputs
    a = zeros(TM, TK), b = zeros(TK, TN), c = zeros(TN, TP)
    d = zeros(TM, TP)  // allocate shared memory buffer for output

    // ============ climb up the memory hierarchy =============

    // collectively load data from global to shared memory
    for c, bs in (BSS, cs):  // Thread-mapped sequential loop
      load_g2s(c)  // load data tile from global to shared memory
      for a, b in (as, bs):  // Thread-mapped sequential loop
        load_g2s(a), load_g2s(b) // load data tile from global to shared memory

        // Load data into thread-local register files
        for a', b' in (a, b):  // a' and b' are register tiles
          load_s2r(a'), load_s2r(b')  // load data tile from shared memory to register file

          // compute with thread-local register file
          p' += mma(a', b')  // [TM, TN] = [TM, TK], [TK, TN]
        
        for c' in c:
          load_s2r(c, c')
          // compute with thread-local register file
          d' += mma(c', p')

    // ============ climb down the memory hierarchy =============
        store_r2s(d', d);   // store from thread-local register file to shared memory
      store_s2g(d, dss)  // store from shared memory to global memory
```

We can generate a complete fused kernel by describing the data flow and manually optimizing macro kernels based on memory hierarchy data structures.

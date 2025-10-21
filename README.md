# hft-ecosystem
High Frequency Trading ecosystem of low-latency C++ libraries

## Common
A library of utilities for use in the upcoming low-latency HFT ecosystem. Contains thread and clock utilities.
Also contains implementations of a memory pool and lock-free queue.

### Memory Pool
works as a means to get a block of memory on the heap to allow faster allocation and deallocation without needing to do it dynamically through standard library data structures.

### Lock-Free Queue
Lightweight single producer, single consumer queue (not thread safe). This will be used in an upcoming logger implementation, to allow using a single logger thread to efficiently write logs outside of a critical path.

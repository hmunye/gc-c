# objgc

A simple object system with **reference counting** and **mark-and-sweep** 
garbage collection implemented in C. Designed as a learning tool for 
understanding garbage collection techniques.

## Reference Counting

**Reference counting** tracks the number of references to an object, incrementing
the count when an object is created or referenced by another object, decrementing 
when an object loses a reference or goes out of scope, and freeing memory when the 
count reaches 0. Downsides to a naive implementation include the inability to track 
cycles and additional runtime overhead managing reference counts of all objects.

## Mark and Sweep

The **mark and sweep** garbage collection strategy works in two phases; the
**mark** phase marks all reachable objects starting from root objects (e.g., global 
variables or stack variables) by setting a flag or tracking them in some other way. 
The **sweep** phase iterates through all allocated objects and frees any unmarked 
objects, as they are no longer accessible or referenced. This method can handle cyclic 
references (objects referencing each other in a cycle) and can be more efficient 
than **reference counting**, as it doesn't require constantly updating reference 
counts during program execution, instead, only updating during garbage collection.
Downsides include higher overall runtime overhead due to object traversal and 
pauses during garbage collection.

## Branches

- `main`: Basic object system implementation.
- `ref_count`: Implements reference counting garbage collection on objects.
- `mark_and_sweep`: Implements mark and sweep garbage collection on objects.

## Build

Run the following command to see available build options:

```bash
make help
```

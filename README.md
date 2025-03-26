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

## Branches

- `main`: Basic object system implementation.
- `ref_count`: Implements reference counting garbage collection on objects.

## Build

Run the following command to see available build options:

```bash
make help
```

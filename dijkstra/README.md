Test case from https://github.com/avinabadasgupta/OpenCL-Graph-Theory : dijtragpu

Original -> original-modified
Using our build scripts

translated -> translated-modified
We remove unused headers (defs.h and cl.h).

final result
Fails to compile via openhipify. We remove some dangling OpenCL intrinsice, and we get a compile. Output tested with cmp -s, isomorphic
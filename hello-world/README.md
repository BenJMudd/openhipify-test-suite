Test case from OpenCL-101: hello-world-from-kernel

Original -> original-modified
We have removed the inbuilt kernel loading code to use our provided auxiliary functions

translated -> translated-modified
we have removed some unused headers, and print statements referencing culled opencl
values. We have removed some comments referencing OpenCL specifics.

final result
Compiles + runs isomorphically to the original program
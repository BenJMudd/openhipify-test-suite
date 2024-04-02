Test case from OpenCL-examples : mandelbrot

Original -> original-modified
Print statement to verify results added

translated -> translated-modified
We remove unused headers (defs.h and cl.h)

final result
Fails to compile via openhipify. We translate all cl_int to hipError_t, but this program used cl_ints as a standard int, and attempted to
iterate. A warning in openhipify was thrown for this. Changing to an int allows compilation, but we get different results, a string of 0s.
investigating further we find the issue is contained in the number of kernels being launched. The opencl version is launching 2048 kernel blocks,
each of size 256. This is a total of 524,288 kernels for this issue, which is completely absurd. We need to investigate further, but OpenCL allows this.
Changing the global size to a more suitable expression, we see an isomorphic binary.
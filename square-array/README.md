Test case from OpenCL-examples : square_array

Original -> original-modified
no changes

translated -> translated-modified
We remove unused headers (defs.h and cl.h).

final result
Fails to compile via openhipify. We translate all cl_int to hipError_t, but this program used cl_ints as a standard int, and attempted to
iterate. A warning in openhipify was thrown for this. Changing to an int produces the same results, but a different binary. The global and
local sizes are different, 256 in the OpenCL version, and 1024 in the HIP version.
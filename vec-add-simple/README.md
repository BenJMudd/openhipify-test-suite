Test case from OpenCL-101: vec-add-simple

Original -> original-modified
We have removed the inbuilt kernel loading code to use our provided auxiliary functions

translated -> translated-modified
We remove unused headers (defs.h and cl.h).

final result
Openhipify fails compilation. This is due to OpenCL allowing a NULL parameter to be passed into kernel launches. HIP
does not, so we get a compilation error. we change this to a 1. We also fail to cull the
array references of cl_device_id* and cl_device_id*. We have designed openhipify for the 
moment to work on a single device, so we missed this. Culling these lines produces an
isomorphic binary.
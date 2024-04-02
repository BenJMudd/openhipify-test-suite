Test case from OpenCL-101: vec-add-simple

Original -> original-modified
We have removed the inbuilt kernel loading code to use our provided auxiliary functions

translated -> translated-modified
We remove unused headers (defs.h and cl.h)

final result
Openhipify fails compilation. This is due to OpenCL allowing a NULL parameter to be passed into kernel launches. HIP
does not, so we get a compilation error. Changing this to a 1 produces an isomorphic binary.
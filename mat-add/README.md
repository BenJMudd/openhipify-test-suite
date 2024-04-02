Test case from OpenCL-101: mat-add

Original -> original-modified
We have removed the inbuilt kernel loading code to use our provided auxiliary functions

translated -> translated-modified
we have removed some unused headers, and uneccesary comments. Large changes have included
removing error handles and return statements. 

final result
Compiles + runs isomorphically to the original program
///////////////// Generated by Openhipify /////////////////
#include "hip/hip_runtime.h"

#include "hello.cl.hpp"
///////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

int main() {

  void *memobj = NULL;

  hipError_t ret = hipSuccess;

  char string[MEM_SIZE];

  /* Create Memory Buffer */
  ret = hipMalloc((void **)&memobj, MEM_SIZE * sizeof(char));

  /* Execute HIP Kernel */
  hipLaunchKernelGGL(hello, dim3(1), dim3(1), 0, 0, (char *)memobj);

  /* Copy results from the memory buffer */
  ret =
      hipMemcpy(string, memobj, MEM_SIZE * sizeof(char), hipMemcpyDeviceToHost);

  /* Display Result */
  puts(string);

  /* Finalization */
  ret = hipFree(memobj);

  return 0;
}

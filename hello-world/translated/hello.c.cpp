///////////////// Generated by Openhipify /////////////////
#include "hip/hip_runtime.h"

#include "hello.cl.hpp"
///////////////////////////////////////////////////////////

#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

int main() {

  void *memobj = NULL;

  hipError_t ret = hipSuccess;

  char string[MEM_SIZE];

  /* Get Platform and Device Info */
  /*  cl_int clGetPlatformIDs(	cl_uint num_entries,
      cl_platform_id *platforms,
      cl_uint *num_platforms)
     input params:
       num_entries: The number of cl_platform_id entries that can be added to
     platforms. Platform is NULL or int number, which's bigger than zero.
      *platforms: Returns a list of OpenCL platforms found.
      The cl_platform_id values returned in platforms can be used to identify
       a specific OpenCL platform.
                                              If platforms argument is NULL,
     this argument is ignored. The number of OpenCL platforms returned is the
     mininum of the value specified by num_entries or the number of OpenCL
     platforms available.

                      num_platforms: Returns the number of OpenCL platforms
     available. If num_platforms is NULL, this argument is ignored.
  */

  printf("platform_id: %ld\n", (int)platform_id);
  printf("ret: %s\n", (int)ret);
  printf("ret_num_platforms: %u\n", (int)ret_num_platforms);
  printf("CL_DEVICE_TYPE_DEFAULT: %d\n", CL_DEVICE_TYPE_DEFAULT);
  printf("device_id: %d\n", (int)device_id);
  printf("ret_num_devices: %d\n", (int)ret_num_devices);

  /* Create OpenCL context */

  printf("context: %s\n", context);

  /* Create Command Queue */

  /* Create Memory Buffer */
  ret = hipMalloc((void **)&memobj, MEM_SIZE * sizeof(char));

  /* Create Kernel Program from the source */

  /* Build Kernel Program */

  /* Create OpenCL Kernel */

  /* Set OpenCL Kernel Parameters */

  /* Execute OpenCL Kernel */
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

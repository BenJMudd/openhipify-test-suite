///////////////// Generated by Openhipify /////////////////
#include "hip/hip_runtime.h"

#include "kernel.cl.hpp"
///////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

/* OpenCL Header file */
#include "defs.h"
#include <CL/cl.h>

int main() {
  /* data on host */
  int *a = NULL;
  int *b = NULL;
  int *c = NULL;

  /* element numbers of each array */
  const int num_elements = 10;

  /* array size */
  size_t data_size = sizeof(int) * num_elements;

  /* allocate mem for array */
  a = (int *)malloc(data_size);
  b = (int *)malloc(data_size);
  c = (int *)malloc(data_size);

  /* init array */
  for (int i = 0; i < num_elements; i++) {
    a[i] = i;
    b[i] = i;
    c[i] = a[i] + b[i];
  }
  for (int i = 0; i < num_elements; i++)
    printf("%d ", c[i]);

  /* get platform number of OpenCL */

  printf("num_platforms: %d\n", (int)num_platforms);

  /* allocate a segment of mem space, so as to store supported platform info */
  cl_platform_id *platforms =
      (cl_platform_id *)malloc(num_platforms * sizeof(cl_platform_id));

  /* get platform info */

  /* get device number on platform */

  /* allocate a segment of mem space, to store device info, supported by
   * platform */
  cl_device_id *devices;
  devices = (cl_device_id *)malloc(num_devices * sizeof(cl_device_id));

  /* get device info */

  /* create OpenCL context, and make relation with device */

  /* create cmd queue, and make relation with device */

  /* create 3 buffer object, to store data of array */
  void *buffer_a, *buffer_b, *buffer_c;
  hipMalloc((void **)&buffer_a, data_size);
  hipMalloc((void **)&buffer_b, data_size);
  hipMalloc((void **)&buffer_c, data_size);

  /* copy array a and b to buffer_a and buffer_b */
  hipMemcpy(buffer_a, a, data_size, hipMemcpyHostToDevice);
  hipMemcpy(buffer_b, b, data_size, hipMemcpyHostToDevice);

  /* create OpenCL kernel, which is used to make vector addition */

  /* parameter transport from host to OpenCL kernel function */

  /* define index space (number of work-items), here only use 1 work group */
  size_t global_work_size[1];

  /* The number of work-items in work-group is `num_elements` */
  global_work_size[0] = num_elements;

  /* execute kernel compute */
  hipLaunchKernelGGL(vec_add, dim3(*(global_work_size)), dim3(*(NULL)), 0, 0,
                     (int *)buffer_a, (int *)buffer_b, (int *)buffer_c);

  /* copy `buffer_c` to `c` in host */
  hipMemcpy(c, buffer_c, data_size, hipMemcpyDeviceToHost);

  for (int i = 0; i < num_elements; i++) {
    printf("%d ", c[i]);
  }

  /* verify result */
  for (int i = 0; i < num_elements; i++) {
    if (c[i] != i + i) {
      printf("Output is incorrect\n");
      break;
    }
  }
  printf("Output is correct\n");

  /* clean device resources */

  hipFree(buffer_a);
  hipFree(buffer_b);
  hipFree(buffer_c);

  /* clean host resources */
  free(a);
  free(b);
  free(c);
  free(platforms);
  free(devices);

  return 0;
}

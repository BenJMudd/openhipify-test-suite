///////////////// Generated by Openhipify /////////////////
#include "hip/hip_runtime.h"

#include "kernelOne.cl.hpp"
///////////////////////////////////////////////////////////

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include "build.h"
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage of %s: [number]\n", argv[0]);
    return 1;
  }
  int n = atoi(argv[1]);

  int32_t ret;
  uint32_t num_devices, num_platforms;

  float a[1] = {(float)n};
  float r[1] = {0};

  // Allocate memory for arguments
  void *ma;
  ret = hipMalloc((void **)&ma, sizeof(a));
  void *mr;
  ret = hipMalloc((void **)&mr, sizeof(r));

  // Write fib arguments
  hipMemcpy(ma, a, sizeof(a), hipMemcpyHostToDevice);
  hipMemcpy(mr, r, sizeof(r), hipMemcpyHostToDevice);

  // Calculate fib
  size_t work[2] = {1, 0};
  hipLaunchKernelGGL(vecAdd, dim3(*(work)), dim3(*(work)), 0, 0,
                     (const float *)ma, (float *)mr);

  // Allocate memory for arguments
  void *ma_one;
  ret = hipMalloc((void **)&ma_one, sizeof(a));
  void *mr_two;
  ret = hipMalloc((void **)&mr_two, sizeof(r));

  // Write fib arguments
  hipMemcpy(ma_one, a, sizeof(a), hipMemcpyHostToDevice);
  hipMemcpy(mr_two, r, sizeof(r), hipMemcpyHostToDevice);

  // Calculate fib
  hipLaunchKernelGGL(vecSub, dim3(*(work)), dim3(*(work)), 0, 0,
                     (const float *)ma_one, (float *)mr_two);

  // Read GPU result
  hipMemcpy(r, mr_two, sizeof(r), hipMemcpyDeviceToHost);

  hipFree(ma);
  hipFree(mr);

  printf("%ld\n", (long)r[0]);
  return 0;
}
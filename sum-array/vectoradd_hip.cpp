/*
Copyright (c) 2015-2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "hip/hip_runtime.h"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#ifdef NDEBUG
#define HIP_ASSERT(x) x
#else
#define HIP_ASSERT(x) (assert((x) == hipSuccess))
#endif

#define WIDTH 10000
#define HEIGHT 10000

#define NUM (WIDTH * HEIGHT)

#define THREADS_PER_BLOCK_X 16
#define THREADS_PER_BLOCK_Y 16
#define THREADS_PER_BLOCK_Z 1

__global__ void
vectoradd_double(double *__restrict__ a, const double *__restrict__ b,
                 const double *__restrict__ c, int width, int height)

{

  int x = hipBlockDim_x * hipBlockIdx_x + hipThreadIdx_x;
  int y = hipBlockDim_y * hipBlockIdx_y + hipThreadIdx_y;

  int i = y * width + x;
  if (i < (width * height)) {
    a[i] = b[i] + c[i];
  }
}

#if 0
__kernel__ void vectoradd_double(double* a, const float* b, const float* c, int width, int height) {

  
  int x = blockDimX * blockIdx.x + threadIdx.x;
  int y = blockDimY * blockIdy.y + threadIdx.y;

  int i = y * width + x;
  if ( i < (width * height)) {
    a[i] = b[i] + c[i];
  }
}
#endif

using namespace std;

int main() {

  double *hostA;
  double *hostB;
  double *hostC;

  double *deviceA;
  double *deviceB;
  double *deviceC;

  hipDeviceProp_t devProp;
  hipGetDeviceProperties(&devProp, 0);
  cout << " System minor " << devProp.minor << endl;
  cout << " System major " << devProp.major << endl;
  cout << " agent prop name " << devProp.name << endl;

  cout << "hip Device prop succeeded " << endl;

  int i;
  int errors;

  hostA = (double *)malloc(NUM * sizeof(double));
  hostB = (double *)malloc(NUM * sizeof(double));
  hostC = (double *)malloc(NUM * sizeof(double));

  // initialize the input data
  for (i = 0; i < NUM; i++) {
    hostB[i] = sinf(i) * sinf(i);
    hostC[i] = cosf(i) * cosf(i);
  }

  HIP_ASSERT(hipMalloc((void **)&deviceA, NUM * sizeof(double)));
  HIP_ASSERT(hipMalloc((void **)&deviceB, NUM * sizeof(double)));
  HIP_ASSERT(hipMalloc((void **)&deviceC, NUM * sizeof(double)));

  HIP_ASSERT(
      hipMemcpy(deviceB, hostB, NUM * sizeof(double), hipMemcpyHostToDevice));
  HIP_ASSERT(
      hipMemcpy(deviceC, hostC, NUM * sizeof(double), hipMemcpyHostToDevice));

  hipLaunchKernelGGL(
      vectoradd_double,
      dim3(WIDTH / THREADS_PER_BLOCK_X, HEIGHT / THREADS_PER_BLOCK_Y),
      dim3(THREADS_PER_BLOCK_X, THREADS_PER_BLOCK_Y), 0, 0, deviceA, deviceB,
      deviceC, WIDTH, HEIGHT);

  HIP_ASSERT(
      hipMemcpy(hostA, deviceA, NUM * sizeof(double), hipMemcpyDeviceToHost));

  // verify the results
  errors = 0;
  for (i = 0; i < NUM; i++) {
    if (hostA[i] != (hostB[i] + hostC[i])) {
      errors++;
    }
  }
  if (errors != 0) {
    printf("FAILED: %d errors\n", errors);
  } else {
    printf("PASSED!\n");
  }

  HIP_ASSERT(hipFree(deviceA));
  HIP_ASSERT(hipFree(deviceB));
  HIP_ASSERT(hipFree(deviceC));

  free(hostA);
  free(hostB);
  free(hostC);

  // hipResetDefaultAccelerator();

  return errors;
}

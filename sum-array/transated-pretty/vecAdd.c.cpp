///////////////// Generated by Openhipify /////////////////
#include "hip/hip_runtime.h"

#include "vecAdd.cl.hpp"
///////////////////////////////////////////////////////////

#define PROGRAM_FILE "vecAdd.cl"
#define KERNEL_FUNC "vecAdd"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Length of vectors
  unsigned int n = 99999999;

  // Host input vectors
  double *h_a;
  double *h_b;
  // Host output vector
  double *h_c;

  // Device input buffers
  void *d_a;
  void *d_b;
  // Device output buffer
  void *d_c;

  // Size, in bytes, of each vector
  size_t bytes = n * sizeof(double);

  // Allocate memory for each vector on host
  h_a = (double *)malloc(bytes);
  h_b = (double *)malloc(bytes);
  h_c = (double *)malloc(bytes);

  // Initialize vectors on host
  int i;
  for (i = 0; i < n; i++) {
    h_a[i] = sinf(i) * sinf(i);
    h_b[i] = cosf(i) * cosf(i);
  }

  size_t globalSize, localSize;
  hipError_t err = hipSuccess;

  // Number of work items in each local work group
  localSize = 64;

  // Number of total work items - localSize must be devisor
  globalSize = ceil(n / (float)localSize) * localSize;

  // Create the input and output arrays in device memory for our calculation
  hipMalloc((void **)&d_a, bytes);
  hipMalloc((void **)&d_b, bytes);
  hipMalloc((void **)&d_c, bytes);

  // Write our data set into the input array in device memory
  err = hipMemcpy(d_a, h_a, bytes, hipMemcpyHostToDevice);
  err = hipMemcpy(d_b, h_b, bytes, hipMemcpyHostToDevice);

  // Execute the kernel over the entire range of the data set
  hipLaunchKernelGGL(vecAdd, dim3(globalSize), dim3(localSize), 0, 0,
                     (double *)d_a, (double *)d_b, (double *)d_c, n);

  // Read the results from the device
  hipMemcpy(h_c, d_c, bytes, hipMemcpyDeviceToHost);

  // Sum up vector c and print result divided by n, this should equal 1 within
  // error
  double sum = 0;
  for (i = 0; i < n; i++)
    sum += h_c[i];
  printf("final result: %f\n", sum / n);

  // release HIP resources
  hipFree(d_a);
  hipFree(d_b);
  hipFree(d_c);

  // release host memory
  free(h_a);
  free(h_b);
  free(h_c);

  return 0;
}
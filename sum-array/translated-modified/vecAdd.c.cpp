///////////////// Generated by Openhipify /////////////////
#include "hip/hip_runtime.h"

#include "vecAdd.cl.hpp"
///////////////////////////////////////////////////////////

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define WIDTH 10000
#define HEIGHT 10000
#define NUM (WIDTH * HEIGHT)

#define THREADS_PER_BLOCK_X 16
#define THREADS_PER_BLOCK_Y 16
#define THREADS_PER_BLOCK_Z 1

int main(int argc, char *argv[]) {
  // Length of vectors
  unsigned int n = 100000000;

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

  // OpenCL platform
  // device ID
  // context
  // command queue
  // program
  // kernel

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

  hipError_t err = hipSuccess;

  // Number of work items in each local work group
  size_t globalSize[2] = {WIDTH / THREADS_PER_BLOCK_X,
                          HEIGHT / THREADS_PER_BLOCK_Y};

  // Number of total work items - localSize must be devisor

  size_t localSize[2] = {THREADS_PER_BLOCK_X, THREADS_PER_BLOCK_Y};

  // Bind to platform

  // Get ID for the device

  // Create a context

  // Create a command queue

  // Create the compute program from the source buffer

  // Build the program executable

  // Create the compute kernel in the program we wish to run

  // Create the input and output arrays in device memory for our calculation
  hipMalloc((void **)&d_a, bytes);
  hipMalloc((void **)&d_b, bytes);
  hipMalloc((void **)&d_c, bytes);

  // Write our data set into the input array in device memory
  err = hipMemcpy(d_a, h_a, bytes, hipMemcpyHostToDevice);
  err = hipMemcpy(d_b, h_b, bytes, hipMemcpyHostToDevice);

  // Set the arguments to our compute kernel

  // Execute the kernel over the entire range of the data set
  hipLaunchKernelGGL(vecAdd, dim3(globalSize[0], globalSize[1]),
                     dim3(localSize[0], localSize[1]), 0, 0, (double *)d_a,
                     (double *)d_b, (double *)d_c, n);

  // Wait for the command queue to get serviced before reading back results

  // Read the results from the device
  hipMemcpy(h_c, d_c, bytes, hipMemcpyDeviceToHost);

  // Sum up vector c and print result divided by n, this should equal 1 within
  // error
  double sum = 0;
  for (i = 0; i < n; i++)
    sum += h_c[i];
  // printf("final result: %f\n", sum / n);

  // release OpenCL resources
  hipFree(d_a);
  hipFree(d_b);
  hipFree(d_c);

  // release host memory
  free(h_a);
  free(h_b);
  free(h_c);

  return 0;
}
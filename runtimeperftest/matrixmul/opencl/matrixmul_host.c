////////////////////////////////////////////////////////////////////////////////

#include "defs.h"
#include <CL/cl.h>
#include <fcntl.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
////////////////////////////////////////////////////////////////////////////////
#define HB WA
#define WC WB
#define HC HA
////////////////////////////////////////////////////////////////////////////////

double CurTime() {
  struct timespec now;
  timespec_get(&now, TIME_UTC);
  return (double)now.tv_sec + (double)now.tv_nsec * 1e-9;
}

// Allocates a matrix with random float entries.
void randomMemInit(float *data, int size) {
  int i;

  for (i = 0; i < size; ++i)
    data[i] = rand() / (float)RAND_MAX;
}

void printResults(size_t size_C, float *h_C, int arrSize) {

  printf("\n\nMatrix C (Results)\n");
  int i;
  for (i = 0; i < size_C; i++) {
    printf("%f ", h_C[i]);
    if (((i + 1) % arrSize) == 0)
      printf("\n");
  }
  printf("\n");
}

int RunKernels(int arrSize, double *setTimeTotal, double *kernelTimeTotal) {

  double start, setupEnd, kernelEnd;

  start = CurTime();
  int err; // error code returned from api calls

  cl_device_id device_id;    // compute device id
  cl_context context;        // compute context
  cl_command_queue commands; // compute command queue
  cl_program program;        // compute program
  cl_kernel kernel;          // compute kernel

  // OpenCL device memory for matrices
  cl_mem d_A;
  cl_mem d_B;
  cl_mem d_C;

  // set seed for rand()
  srand(2014);

  // Allocate host memory for matrices A and B
  unsigned int size_A = arrSize * arrSize;
  unsigned int mem_size_A = sizeof(float) * size_A;
  float *h_A = (float *)malloc(mem_size_A);

  unsigned int size_B = arrSize * arrSize;
  unsigned int mem_size_B = sizeof(float) * size_B;
  float *h_B = (float *)malloc(mem_size_B);

  // Initialize host memory
  randomMemInit(h_A, size_A);
  randomMemInit(h_B, size_B);

  // Allocate host memory for the result C
  unsigned int size_C = arrSize * arrSize;
  unsigned int mem_size_C = sizeof(float) * size_C;
  float *h_C = (float *)malloc(mem_size_C);

  device_id = create_device();
  // Create a compute context
  context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  if (!context) {
    printf("Error: Failed to create a compute context!\n");
    return EXIT_FAILURE;
  }

  // Create a command commands
  commands = clCreateCommandQueue(context, device_id, 0, &err);
  if (!commands) {
    printf("Error: Failed to create a command commands!\n");
    return EXIT_FAILURE;
  }

  program = build_program(context, device_id, "matrixmul_kernel.cl");
  // Create the compute kernel in the program we wish to run
  //
  kernel = clCreateKernel(program, "matrixMul", &err);
  if (!kernel || err != CL_SUCCESS) {
    printf("Error: Failed to create compute kernel!\n");
    exit(1);
  }

  // Create the input and output arrays in device memory for our calculation
  d_C = clCreateBuffer(context, CL_MEM_READ_WRITE, mem_size_A, NULL, &err);
  d_A = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                       mem_size_A, h_A, &err);
  d_B = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                       mem_size_B, h_B, &err);

  if (!d_A || !d_B || !d_C) {
    printf("Error: Failed to allocate device memory!\n");
    exit(1);
  }

  err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&d_C);
  err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&d_A);
  err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&d_B);
  err |= clSetKernelArg(kernel, 3, sizeof(int), (void *)&arrSize);
  err |= clSetKernelArg(kernel, 4, sizeof(int), (void *)&arrSize);

  if (err != CL_SUCCESS) {
    printf("Error: Failed to set kernel arguments! %d\n", err);
    exit(1);
  }

  size_t blockSize[2] = {16, 16};
  size_t workSize[2] = {arrSize, arrSize};
  setupEnd = CurTime();
  err = clEnqueueNDRangeKernel(commands, kernel, 2, NULL, workSize, blockSize,
                               0, NULL, NULL);

  if (err != CL_SUCCESS) {
    printf("Error: Failed to execute kernel! %d\n", err);
    exit(1);
  }

  // Retrieve result from device
  err = clEnqueueReadBuffer(commands, d_C, CL_TRUE, 0, mem_size_C, h_C, 0, NULL,
                            NULL);

  if (err != CL_SUCCESS) {
    printf("Error: Failed to read output array! %d\n", err);
    exit(1);
  }

  kernelEnd = CurTime();
  // print out the results
  *setTimeTotal += setupEnd - start;
  *kernelTimeTotal += kernelEnd - setupEnd;
  // printResults(size_C, h_C, arrSize);
  // Shutdown and cleanup
  free(h_A);
  free(h_B);
  free(h_C);

  clReleaseMemObject(d_A);
  clReleaseMemObject(d_C);
  clReleaseMemObject(d_B);

  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(commands);
  clReleaseContext(context);
}

int main(int argc, char **argv) {
  int arrSize = atoi(argv[1]);
  int iters = atoi(argv[2]);
  double setupTime = 0;
  double kernelTime = 0;
  for (int i = 0; i < iters; ++i) {
    RunKernels(arrSize, &setupTime, &kernelTime);
  }
  printf("setup time: %f\n", setupTime);
  printf("kernel time: %f\n", kernelTime);
  return 0;
}

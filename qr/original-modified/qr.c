#define _CRT_SECURE_NO_WARNINGS
#define PROGRAM_FILE "qr.cl"
#define KERNEL_FUNC "qr"

#define MATRIX_DIM 256

#include "defs.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

int main() {

  /* Host/device data structures */
  cl_device_id device;
  cl_context context;
  cl_command_queue queue;
  cl_program program;
  cl_kernel kernel;
  size_t global_size, local_size;
  cl_int err, i, j, k, check;

  /* Data and buffers */
  float a_mat[MATRIX_DIM][MATRIX_DIM], q_mat[MATRIX_DIM][MATRIX_DIM],
      r_mat[MATRIX_DIM][MATRIX_DIM], check_mat[MATRIX_DIM][MATRIX_DIM];
  cl_mem a_buffer, q_buffer, p_buffer, prod_buffer;

  /* Initialize A matrix */
  srand((unsigned int)time(0));
  for (i = 0; i < MATRIX_DIM; i++) {
    for (j = 0; j < MATRIX_DIM; j++) {
      a_mat[i][j] = (float)rand() / RAND_MAX;
      check_mat[i][j] = 0.0f;
    }
  }

  /* Create a device and context */
  device = create_device();
  context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
  if (err < 0) {
    perror("Couldn't create a context");
    exit(1);
  }

  /* Build the program */
  program = build_program(context, device, PROGRAM_FILE);

  /* Create a kernel */
  kernel = clCreateKernel(program, KERNEL_FUNC, &err);
  if (err < 0) {
    perror("Couldn't create a kernel");
    exit(1);
  };

  /* Create buffer */
  a_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                            sizeof(a_mat), a_mat, &err);
  if (err < 0) {
    perror("Couldn't create a buffer");
    exit(1);
  };
  q_buffer =
      clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(q_mat), NULL, NULL);
  p_buffer =
      clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(q_mat), NULL, NULL);
  prod_buffer =
      clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(q_mat), NULL, NULL);

  /* Create kernel arguments */
  err = clSetKernelArg(kernel, 0, MATRIX_DIM * sizeof(float), NULL);
  err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &a_buffer);
  err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &q_buffer);
  err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &p_buffer);
  err |= clSetKernelArg(kernel, 4, sizeof(cl_mem), &prod_buffer);
  if (err < 0) {
    printf("Couldn't set a kernel argument");
    exit(1);
  };

  /* Create a command queue */
  queue = clCreateCommandQueue(context, device, 0, &err);
  if (err < 0) {
    perror("Couldn't create a command queue");
    exit(1);
  };

  /* Enqueue kernel */
  global_size = MATRIX_DIM;
  local_size = MATRIX_DIM;
  err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size,
                               &local_size, 0, NULL, NULL);
  if (err < 0) {
    perror("Couldn't enqueue the kernel");
    exit(1);
  }

  /* Read the results */
  err = clEnqueueReadBuffer(queue, q_buffer, CL_TRUE, 0, sizeof(q_mat), q_mat,
                            0, NULL, NULL);
  err |= clEnqueueReadBuffer(queue, a_buffer, CL_TRUE, 0, sizeof(r_mat), r_mat,
                             0, NULL, NULL);
  if (err < 0) {
    perror("Couldn't read the buffers");
    exit(1);
  }

  /* Compute product of Q and R */
  for (i = 0; i < MATRIX_DIM; i++) {
    for (j = 0; j < MATRIX_DIM; j++) {
      for (k = 0; k < MATRIX_DIM; k++) {
        check_mat[i][j] += q_mat[i][k] * r_mat[k][j];
      }
    }
  }

  /* Check data */
  check = 1;
  for (i = 0; i < MATRIX_DIM; i++) {
    for (j = 0; j < MATRIX_DIM; j++) {
      if (fabs(a_mat[i][j] - check_mat[i][j]) > 0.01f) {
        check = 0;
        break;
      }
    }
  }
  if (check)
    printf("QR decomposition check succeeded.\n");
  else
    printf("QR decomposition check failed.\n");

  /* Deallocate resources */
  clReleaseMemObject(a_buffer);
  clReleaseMemObject(q_buffer);
  clReleaseMemObject(p_buffer);
  clReleaseMemObject(prod_buffer);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(queue);
  clReleaseProgram(program);
  clReleaseContext(context);
  return 0;
}

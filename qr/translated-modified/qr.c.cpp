///////////////// Generated by Openhipify /////////////////
#include "hip/hip_runtime.h"

#include "qr.cl.hpp"
///////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS
#define PROGRAM_FILE "qr.cl"
#define KERNEL_FUNC "qr"

#define MATRIX_DIM 256

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {

  /* Host/device data structures */

  size_t global_size, local_size;
  hipError_t err;
  int i, j, k, check;

  /* Data and buffers */
  float a_mat[MATRIX_DIM][MATRIX_DIM], q_mat[MATRIX_DIM][MATRIX_DIM],
      r_mat[MATRIX_DIM][MATRIX_DIM], check_mat[MATRIX_DIM][MATRIX_DIM];
  void *a_buffer, *q_buffer, *p_buffer, *prod_buffer, *u_buffer;

  /* Initialize A matrix */
  srand((unsigned int)time(0));
  for (i = 0; i < MATRIX_DIM; i++) {
    for (j = 0; j < MATRIX_DIM; j++) {
      a_mat[i][j] = (float)rand() / RAND_MAX;
      check_mat[i][j] = 0.0f;
    }
  }

  /* Create a device and context */

  if (err < hipSuccess) {
    perror("Couldn't create a context");
    exit(1);
  }

  /* Build the program */

  /* Create a kernel */

  if (err < hipSuccess) {
    perror("Couldn't create a kernel");
    exit(1);
  }

  /* Create buffer */
  err = hipMalloc((void **)&a_buffer, sizeof(a_mat));
  hipMemcpy(a_buffer, a_mat, sizeof(a_mat), hipMemcpyHostToDevice);
  if (err < hipSuccess) {
    perror("Couldn't create a buffer");
    exit(1);
  }

  hipMalloc((void **)&q_buffer, sizeof(q_mat));
  hipMalloc((void **)&u_buffer, sizeof(q_mat));
  hipMalloc((void **)&p_buffer, sizeof(q_mat));
  hipMalloc((void **)&prod_buffer, sizeof(q_mat));

  /* Create kernel arguments */

  if (err < hipSuccess) {
    printf("Couldn't set a kernel argument");
    exit(1);
  }

  /* Create a command queue */

  if (err < hipSuccess) {
    perror("Couldn't create a command queue");
    exit(1);
  }

  /* Enqueue kernel */
  global_size = MATRIX_DIM;
  local_size = MATRIX_DIM;
  hipLaunchKernelGGL(qr, dim3(global_size), dim3(local_size), 0, 0,
                     (float *)u_buffer, (float *)a_buffer, (float *)q_buffer,
                     (float *)p_buffer, (float *)prod_buffer);
  if (err < hipSuccess) {
    perror("Couldn't enqueue the kernel");
    exit(1);
  }
  printf("Got here");
  hipDeviceSynchronize();
  /* Read the results */
  err = hipMemcpy(q_mat, q_buffer, sizeof(q_mat), hipMemcpyDeviceToHost);
  err = hipMemcpy(r_mat, a_buffer, sizeof(r_mat), hipMemcpyDeviceToHost);
  if (err < hipSuccess) {
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
  hipFree(a_buffer);
  hipFree(q_buffer);
  hipFree(p_buffer);
  hipFree(prod_buffer);

  return 0;
}
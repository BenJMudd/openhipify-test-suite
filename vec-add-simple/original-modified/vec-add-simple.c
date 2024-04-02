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

  cl_device_id device = create_device();
  /* create OpenCL context, and make relation with device */
  cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);

  /* create cmd queue, and make relation with device */
  cl_command_queue cmd_queue = clCreateCommandQueue(context, device, 0, NULL);

  /* create 3 buffer object, to store data of array */
  cl_mem buffer_a, buffer_b, buffer_c;
  buffer_a = clCreateBuffer(context, CL_MEM_READ_ONLY, data_size, NULL, NULL);
  buffer_b = clCreateBuffer(context, CL_MEM_READ_ONLY, data_size, NULL, NULL);
  buffer_c = clCreateBuffer(context, CL_MEM_READ_WRITE, data_size, NULL, NULL);

  /* copy array a and b to buffer_a and buffer_b */
  clEnqueueWriteBuffer(cmd_queue, buffer_a, CL_FALSE, 0, data_size, a, 0, NULL,
                       NULL);
  clEnqueueWriteBuffer(cmd_queue, buffer_b, CL_FALSE, 0, data_size, b, 0, NULL,
                       NULL);

  /* create OpenCL program from source code */
  cl_program program = build_program(context, device, "kernel.cl");

  /* create OpenCL kernel, which is used to make vector addition */
  cl_kernel kernel = clCreateKernel(program, "vec_add", NULL);

  /* parameter transport from host to OpenCL kernel function */
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_a);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer_b);
  clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer_c);

  /* define index space (number of work-items), here only use 1 work group */
  size_t global_work_size[1];

  /* The number of work-items in work-group is `num_elements` */
  global_work_size[0] = num_elements;

  /* execute kernel compute */
  clEnqueueNDRangeKernel(cmd_queue, kernel, 1, NULL, global_work_size, NULL, 0,
                         NULL, NULL);

  /* copy `buffer_c` to `c` in host */
  clEnqueueReadBuffer(cmd_queue, buffer_c, CL_TRUE, 0, data_size, c, 0, NULL,
                      NULL);

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
  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseCommandQueue(cmd_queue);
  clReleaseMemObject(buffer_a);
  clReleaseMemObject(buffer_b);
  clReleaseMemObject(buffer_c);
  clReleaseContext(context);

  /* clean host resources */
  free(a);
  free(b);
  free(c);

  return 0;
}

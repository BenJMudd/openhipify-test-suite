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

  cl_platform_id platform_id;
  int32_t ret;
  uint32_t num_devices, num_platforms;

  clGetPlatformIDs(1, &platform_id, &num_platforms);

  cl_device_id device_id;
  clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id,
                 &num_devices);

  cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

  cl_command_queue command_queue =
      clCreateCommandQueue(context, device_id, 0, &ret);

  cl_program program = build_program(context, device_id, "kernel.cl");

  cl_kernel kernel = clCreateKernel(program, "vecAdd", &ret);

  float a[1] = {(float)n};
  float r[1] = {0};

  // Allocate memory for arguments
  cl_mem ma = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(a), NULL, &ret);
  cl_mem mr = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(r), NULL, &ret);

  // Write fib arguments
  clEnqueueWriteBuffer(command_queue, ma, CL_TRUE, 0, sizeof(a), a, 0, NULL,
                       NULL);
  clEnqueueWriteBuffer(command_queue, mr, CL_TRUE, 0, sizeof(r), r, 0, NULL,
                       NULL);

  clSetKernelArg(kernel, 0, sizeof(ma), (void *)&ma);
  clSetKernelArg(kernel, 1, sizeof(mr), (void *)&mr);

  // Calculate fib
  size_t work[2] = {1, 0};
  clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, work, work, 0, NULL,
                         NULL);

  cl_kernel kernelSub = clCreateKernel(program, "vecSub", &ret);

  // Allocate memory for arguments
  cl_mem ma_one =
      clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(a), NULL, &ret);
  cl_mem mr_two =
      clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(r), NULL, &ret);

  // Write fib arguments
  clEnqueueWriteBuffer(command_queue, ma_one, CL_TRUE, 0, sizeof(a), a, 0, NULL,
                       NULL);
  clEnqueueWriteBuffer(command_queue, mr_two, CL_TRUE, 0, sizeof(r), r, 0, NULL,
                       NULL);

  clSetKernelArg(kernelSub, 0, sizeof(ma_one), (void *)&ma_one);
  clSetKernelArg(kernelSub, 1, sizeof(mr_two), (void *)&mr_two);

  // Calculate fib
  clEnqueueNDRangeKernel(command_queue, kernelSub, 1, NULL, work, work, 0, NULL,
                         NULL);

  // Read GPU result
  clEnqueueReadBuffer(command_queue, mr_two, CL_TRUE, 0, sizeof(r), r, 0, NULL,
                      NULL);

  clFlush(command_queue);
  clFinish(command_queue);
  clReleaseKernel(kernel);
  clReleaseKernel(kernelSub);
  clReleaseProgram(program);
  clReleaseCommandQueue(command_queue);
  clReleaseContext(context);

  clReleaseMemObject(ma);
  clReleaseMemObject(mr);

  printf("%ld\n", (long)r[0]);
  return 0;
}
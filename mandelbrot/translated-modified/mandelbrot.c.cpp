///////////////// Generated by Openhipify /////////////////
#include "hip/hip_runtime.h"

#include "mandelbrot.cl.hpp"
///////////////////////////////////////////////////////////

#define PROGRAM_FILE "mandelbrot.cl"
#define KERNEL_FUNC "solve_mandelbrot"
#define WG_SIZE 256 // Workgroup size

int main() {

  /* OpenCL structures */

  hipError_t err;
  int i;
  size_t local_size, global_size;

  /* Data and buffers    */
  // Define Mandelbrot Settings
  int iterations = 99999999;
  float x_min = -2;
  float x_max = 2;
  float y_min = -1.5f;
  float y_max = 1.5f;
  float x_step = .002f;
  float y_step = .002f;

  // Create Linear Vector of Coordinates
  int nreals, nimags;
  float *reals, *imags; // Host input arrays
  int *ans;             // Host output array
  nimags = (y_max - y_min) / y_step;
  nreals = (x_max - x_min) / x_step;
  reals = (float *)malloc(sizeof(float) * nreals);
  imags = (float *)malloc(sizeof(float) * nimags);
  ans = (int *)malloc(sizeof(int) * nreals);

  for (i = 0; i < nreals; i++) {
    reals[i] = reals[i] + i * x_step;
  }

  for (i = 0; i < nimags; i++) {
    imags[i] = imags[i] + i * y_step;
  }

  // Device input and output buffers
  void *dreals, *dimags, *dans;

  /* Create device and context   */

  /* Build program */

  /* Create a command queue */

  /* Create data buffer
  Create the input and output arrays in device memory for our
  calculation. 'd' below stands for 'device'.
  */
  hipMalloc((void **)&dreals, sizeof(float) * nreals);
  hipMalloc((void **)&dimags, sizeof(float) * nimags);
  hipMalloc((void **)&dans, sizeof(int) * nreals);

  // Write our data set into the input array in device memory
  err = hipMemcpy(dreals, reals, sizeof(float) * nreals, hipMemcpyHostToDevice);
  err = hipMemcpy(dimags, imags, sizeof(float) * nimags, hipMemcpyHostToDevice);

  /* Create a kernel */

  /* Create kernel arguments    */

  // Number of work items in each local work group
  local_size = WG_SIZE;
  // Number of total work items - localSize must be devisor
  global_size = (nreals - 1) / local_size + 1;
  // size_t global_size[3] = {ARRAY_SIZE, 0, 0}; // for 3D data
  // size_t local_size[3] = {WG_SIZE, 0, 0};
  /* Enqueue kernel    */
  hipLaunchKernelGGL(solve_mandelbrot, dim3(global_size), dim3(local_size), 0,
                     0, (const float *)dreals, (const float *)dimags,
                     iterations, (int *)dans);

  /* Wait for the command queue to get serviced before reading
  back results */

  /* Read the kernel's output    */
  hipMemcpy(ans, dans, sizeof(int) * nreals,
            hipMemcpyDeviceToHost); // <=====GET OUTPUT

  // for (int i = 0; i < nreals; i++) {
  //   printf("%d ", ans[i]);
  // }

  /* Deallocate resources */

  hipFree(dreals);
  hipFree(dimags);
  hipFree(dans);

  return 0;
}

///////////////// Generated by Openhipify /////////////////
#include "hip/hip_runtime.h"

#include "dijkstragpu_kernel.cl.hpp"
///////////////////////////////////////////////////////////

__global__ void dijkstragpu(float *Matrix, float *Visited, float *Min,
                            int *Index, float *Array, int *Path, float *lmat,
                            float *larr) {
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
  size_t id = hipBlockDim_x * hipBlockIdx_x + hipThreadIdx_x;
  size_t lid = hipThreadIdx_x;
  size_t ls = hipBlockDim_x;
  size_t gid = hipBlockIdx_x;
  size_t gs = get_global_size(0);
  __private float min, mid;
  __private int i, j, index;
  if (id == 0) {
    for (j = 0; j < gs; j++) {
      min = 999999;
      for (i = 0; i < gs; i++) {
        if (Visited[i] == 0) {
          if (Matrix[0 * gs + i] < min) {
            min = Matrix[0 * gs + i];
            index = i;
          }
        }
      }
      Visited[index] = 1;
      for (i = 0; i < gs; i++) {
        if (Matrix[0 * gs + i] >
            Matrix[0 * gs + index] + Matrix[index * gs + i]) {
          Matrix[0 * gs + i] = Matrix[0 * gs + index] + Matrix[index * gs + i];
        }
      }
    }
  }
}

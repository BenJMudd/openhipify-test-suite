///////////////// Generated by Openhipify /////////////////
#include "hip/hip_runtime.h"

#include "kernelTwo.cl.hpp"
///////////////////////////////////////////////////////////

__global__ void pScan(const float *a, float *r) {
  unsigned int id = hipBlockDim_x * hipBlockIdx_x + hipThreadIdx_x;
}
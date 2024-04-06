// OpenCL kernel. Each work item takes care of one element of c
__kernel void vecAdd(  __global double *a,                    
                       __global double *b,                   
                       __global double *c,                     
                       const unsigned int n)                   
{                                                             
    //Get our global thread ID                                
    int x = get_global_id(0);      
    int y = get_global_id(1);                    
                                                               
  int i = y * 10000 + x;
  if (i < (100000000)) {
    a[i] = b[i] + c[i];
  }
}                                                              
  
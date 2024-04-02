__kernel void vec_add(__global int *a, __global int *b, __global int *c) {
  /* Get unique id of work-item */
  int idx = get_global_id(0);
  //printf('%d ', idx);
  /* make addition for `a` and `b`, store result into `c` */
  c[idx] = a[idx] + b[idx];
}
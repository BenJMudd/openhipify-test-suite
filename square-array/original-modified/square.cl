__kernel void square(__global float* input, __global float* output, int n) {
	int i = get_global_id(0);

for(int i = 0; i < 200; i++) {
		output[i]=input[i]*input[i];
}
}

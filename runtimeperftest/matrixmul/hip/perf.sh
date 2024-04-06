#!/bin/bash

for i in $(seq 1024 1024 16384);
do
	echo "ARR_SIZE:$i"
	echo "###HIP"
	perf stat ./hip/hip $i 1
	echo "###"
	echo "###OPENCL"
	perf stat ./opencl/opencl $i 1
	echo "###" 
done


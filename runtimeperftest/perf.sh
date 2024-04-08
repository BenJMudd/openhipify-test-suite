#!/bin/sh

for i in $(seq 1024 1024 16384);
do
	echo "ARR_SIZE:$i"
	echo "###HIP"
	perf stat -r 20 ./hip/hip $i 1
	echo "###"
       sleep 2	
	echo "###OPENCL"
	perf stat -r 20 ./opencl/opencl $i 1
	echo "###"
       sleep 2	
done


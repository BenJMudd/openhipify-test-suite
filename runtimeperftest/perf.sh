#!/bin/sh

for i in $(seq 10 10 100);
do
	echo "ITERATIONS:$i"
	echo "###HIP"
	perf stat ./hip/hip 4096 $i
	echo "###"
       sleep 2	
	echo "###OPENCL"
	perf stat ./opencl/opencl 4096 $i
	echo "###"
       sleep 2	
done


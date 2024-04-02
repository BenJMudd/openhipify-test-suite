
cd original-modified
openhipify dijkstragpu_kernel.cl dijkstragpu.c -- && mv *pp ../translated/ && clang-format -i ../translated/* && cp ../translated/* ../translated-modified/
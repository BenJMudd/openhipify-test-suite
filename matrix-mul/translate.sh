cd original-modified
openhipify matrixmul_host.c matrixmul_kernel.cl -- && mv *pp ../translated/ && clang-format -i ../translated/* 
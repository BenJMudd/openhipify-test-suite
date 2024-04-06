
cd original
openhipify hostOne.c hostTwo.c hostThree.c kernelOne.cl kernelTwo.cl -- && mv *pp ../translated/ && clang-format -i ../translated/*
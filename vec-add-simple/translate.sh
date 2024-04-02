cd original-modified
openhipify kernel.cl vec-add-simple.c -- && mv *pp ../translated/ && clang-format -i ../translated/* && cp ../translated/* ../translated-modified/
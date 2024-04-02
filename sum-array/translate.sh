
cd original-modified
openhipify vecAdd.c vecAdd.cl -- && mv *pp ../translated/ && clang-format -i ../translated/* && cp ../translated/* ../translated-modified/
cd original-modified
openhipify hello.c hello.cl -- && mv *pp ../translated/ && clang-format -i ../translated/* && cp ../translated/* ../translated-modified/
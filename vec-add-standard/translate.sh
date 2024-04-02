
cd original-modified
openhipify vec-add-standard.c vec-add-standard.cl -- && mv *pp ../translated/ && clang-format -i ../translated/* && cp ../translated/* ../translated-modified/
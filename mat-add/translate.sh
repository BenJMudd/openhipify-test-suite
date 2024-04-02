cd original-modified
openhipify mat-add.c mat-add.cl -- && mv *pp ../translated/ && clang-format -i ../translated/* && cp ../translated/* ../translated-modified/
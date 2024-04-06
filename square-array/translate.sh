
cd original-modified
openhipify square.c square.cl -- && mv *pp ../translated/ && clang-format -i ../translated/* 
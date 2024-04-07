
cd original-modified
openhipify qr.c qr.cl -- && mv *pp ../translated/ && clang-format -i ../translated/* && cp ../translated/* ../translated-modified/
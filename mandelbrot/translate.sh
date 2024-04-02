
cd original-modified
openhipify mandelbrot.c mandelbrot.cl -- && mv *pp ../translated/ && clang-format -i ../translated/* && cp ../translated/* ../translated-modified/
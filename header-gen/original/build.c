#define PROGRAM_FILE "vecAdd.cl"
#define KERNEL_FUNC "vecAdd"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "build.h"

/* Create program from a file and compile it */
cl_program build_program(cl_context ctx, cl_device_id dev,
                         const char *filename) {

  FILE *program_handle;
  char *program_buffer, *program_log;
  size_t program_size, log_size;
  int err;

  /* Read program file and place content into buffer */
  program_handle = fopen(filename, "r");
  if (program_handle == NULL) {
    perror("Couldn't find the program file");
    exit(1);
  }
  fseek(program_handle, 0, SEEK_END);
  program_size = ftell(program_handle);
  rewind(program_handle);
  program_buffer = (char *)malloc(program_size + 1);
  program_buffer[program_size] = '\0';
  fread(program_buffer, sizeof(char), program_size, program_handle);
  fclose(program_handle);

  /* Create program from file

  Creates a program from the source code in the add_numbers.cl file.
  Specifically, the code reads the file's content into a char array
  called program_buffer, and then calls clCreateProgramWithSource.
  */
  cl_program program = clCreateProgramWithSource(
      ctx, 1, (const char **)&program_buffer, &program_size, &err);
  if (err < 0) {
    perror("Couldn't create the program");
    exit(1);
  }
  free(program_buffer);

  /* Build program

  The fourth parameter accepts options that configure the compilation.
  These are similar to the flags used by gcc. For example, you can
  define a macro with the option -DMACRO=VALUE and turn off optimization
  with -cl-opt-disable.
  */
  clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

  return program;
}
#pragma once

#include <CL/cl.h>
cl_program build_program(cl_context ctx, cl_device_id dev,
                         const char *filename);
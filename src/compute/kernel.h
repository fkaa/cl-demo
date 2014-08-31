#ifndef KERNEL_H
#define KERNEL_H

#include <cmath>
#include <string>
#include <fstream>
#include <streambuf>
#include <stdint.h>

#include "compute/mcl.h"

class Kernel {
 public:
  Kernel(const char* path, const char* meth);
  ~Kernel();
  void set_arg(uint32_t idx, size_t size, void* arg);
  void exec(uint32_t group_count, uint32_t elements);
 private:
  cl_program program;
  cl_kernel kernel;
  int group_limit;
};

#endif

#ifndef KERNEL_H
#define KERNEL_H

#include <vector>

#include "compute/mcl.h"

class Kernel {
 public:
  Kernel(const char* path, const char* meth);
  ~Kernel();
  void run();
 private:
  cl_program program;
  cl_kernel kernel;
  int group_limit;
};

#endif

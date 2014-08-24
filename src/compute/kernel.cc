#include "compute/kernel.h"

Kernel::Kernel(const char* path, const char* meth) {
  std::vector<char> data;
  if (FILE* fp = fopen(path, "r")) {
    char buf[1024];
    while (size_t len = fread(buf, 1, sizeof(buf), fp)) {
      data.insert(data.end(), buf, buf + len);
    }
    fclose(fp);
  }
  const char* src = &data[0];

  int err;
  program = clCreateProgramWithSource(MCL::device_context(), 1, &src, nullptr, &err);
  if (!program || err != CL_SUCCESS) {
    Log::e("Failed to create OpenCL program: '%s' [%i]", path, err);
  }
  
  err = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
  if (err != CL_SUCCESS) {
    size_t len;
    char buf[2048];
    clGetProgramBuildInfo(program, MCL::device_id(), CL_PROGRAM_BUILD_LOG, sizeof(buf), buf, &len);

    Log::e("Failed to compile OpenCL program: '%s' [%i]\n%s", path, err, buf);
  }

  kernel = clCreateKernel(program, meth, &err);
  if (!kernel || err != CL_SUCCESS) {
    Log::e("Failed to create OpenCL kernel: '%s' [%i]", path, err);
  }

  size_t m_groups = 1;
  err = clGetKernelWorkGroupInfo(kernel, MCL::device_id(), CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &m_groups, nullptr);
  if (err != CL_SUCCESS) {
    Log::e("Failed to get kernel work group info: '%s' [%i]", path, err);
  }
  group_limit = m_groups;
}

Kernel::~Kernel() {
  clReleaseKernel(kernel);
  clReleaseProgram(program);
}

void Kernel::run() {

}

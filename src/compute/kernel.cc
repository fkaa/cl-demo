#include "compute/kernel.h"

Kernel::Kernel(const char* path, const char* meth) {
  std::ifstream t(path);
  std::string str;

  t.seekg(0, std::ios::end);   
  str.reserve(t.tellg());
  t.seekg(0, std::ios::beg);
  str.assign((std::istreambuf_iterator<char>(t)),
             std::istreambuf_iterator<char>());

  const char* src = str.c_str();

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

void Kernel::set_arg(uint32_t idx, size_t size, void* arg) {
  if (clSetKernelArg(kernel, idx, size, arg)) {
    Log::w("Failed to set kernel arg: [idx=%i,size=%i,obj=%ul]", idx, size, arg);
  }
}

void Kernel::exec(uint32_t group_count, uint32_t elements) {
  size_t global[1];
  global[0] = elements;

  if (clEnqueueNDRangeKernel(MCL::device_queue(), kernel, 1, nullptr, global, nullptr, 0, nullptr, nullptr)) {
    Log::e("Failed to execute kernel: [max=%i,size=%i]", group_limit, group_count);
  }
}

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

void Kernel::set_arg(int idx, size_t size, void* arg) {
  if (clSetKernelArg(kernel, idx, size, arg)) {
    Log::w("Failed to set kernel arg: [idx=%i,size=%i,obj=%ul]", idx, size, arg);
  }
}

void Kernel::exec(uint group_count, uint elements) {
  size_t local[2];
  size_t global[1];
    // ((a % b) != 0) ? (a / b + 1) : (a / b);
  uint active = fmax(1, group_limit / group_count);
  uint queued = group_limit / active;

  local[0] = active;
  local[1] = queued;

  uint split = ceilf(sqrtf(elements));

  global[0] = elements;//((split % active) != 0 ? split / active + 1 : split / active) * active;
  //global[1] = elements;//((split % queued) != 0 ? split / queued + 1 : split / queued) * queued;

  if (clEnqueueNDRangeKernel(MCL::device_queue(), kernel, 1, nullptr, global, nullptr, 0, nullptr, nullptr)) {
    Log::e("Failed to execute kernel: [max=%i,size=%i]", group_limit, group_count);
  }
}

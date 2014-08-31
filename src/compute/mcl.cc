#include "compute/mcl.h"

cl_int MCL::compute_id;
cl_device_id MCL::compute_device_id;
cl_context MCL::compute_context;
cl_device_type MCL::compute_type;
cl_command_queue MCL::compute_queue;

bool MCL::init(bool use_gpu) {
  int err;
  size_t return_size;

  compute_type = use_gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU;

  cl_platform_id platform;
  clGetPlatformIDs(1, &platform, NULL);

#ifdef __APPLE__
  CGLContextObj kCGLContext = CGLGetCurrentContext();
  CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);

  cl_context_properties prop[] = {
    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)kCGLShareGroup,
    CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
    0
  };
#elif __WIN32
  cl_context_properties prop[] = {
    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentContext(),
    CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
    0
  };
#elif __linux__
  cl_context_properties prop[] = {
    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
    CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(),
    0
  };
#else
  Log::e("No support for this platform, sorry!");
#endif

  compute_context = clCreateContext(prop, 0, 0, clLogMessagesToStdoutAPPLE, 0, 0);
  if (!compute_context) {
    return false;
  }

  unsigned int device_count = 0;
  cl_device_id device_ids[16];

  err = clGetContextInfo(compute_context, CL_CONTEXT_DEVICES, sizeof(device_ids), device_ids, &return_size);
  if (err) {
    return false;
  }

  device_count = return_size / sizeof(cl_device_id);

  cl_device_type type;
  bool found_device = false;
  for (int i = 0; i < device_count; i++) {
    clGetDeviceInfo(device_ids[i], CL_DEVICE_TYPE, sizeof(cl_device_type), &type, nullptr);
    if (type == compute_type) {
      compute_device_id = device_ids[i];
      found_device = true;
      break;
    }
  }

  if (!found_device) {
    return false;
  }

  compute_queue = clCreateCommandQueue(compute_context, compute_device_id, 0, &err);
  if (!compute_queue) {
    return false;
  }

  cl_char vendor_name[1024] = {0};
  cl_char device_name[1024] = {0};
  err = clGetDeviceInfo(compute_device_id, CL_DEVICE_VENDOR, sizeof(vendor_name), vendor_name, &return_size);
  err |= clGetDeviceInfo(compute_device_id, CL_DEVICE_NAME, sizeof(device_name), device_name, &return_size);
  if (err != CL_SUCCESS) {
    Log::w("Failed to retrieve device info!");
  } else {
    Log::v("Connecting to %s %s", vendor_name, device_name);
  }

  return true;
}

void MCL::destroy() {
  clFinish(compute_queue);
  clReleaseContext(compute_context);
}

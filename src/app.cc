#include "app.h"

App::App(int width, int height, int size)
  : use_gpu(false),
    width(width),
    height(height),
    num_particles(size) {
  glGenVertexArrays(1, &particle_vao);
  glBindVertexArray(particle_vao);
  glGenBuffers(1, &particle_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, particle_buffer);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
  glBindVertexArray(0);

  kernel = new Kernel("./data/kernel/particles.cl", "update");

  size_t p_byte_size = sizeof(float) * 2 * num_particles;

  int err;
  velocities_buffer = clCreateBuffer(MCL::device_context(), CL_MEM_READ_WRITE, p_byte_size, nullptr, &err);
  if (!velocities_buffer || err != CL_SUCCESS) {
    Log::e("Failed to create velocities buffer. [%i]", err);
  }
  
  positions_buffer = clCreateFromGLBuffer(MCL::device_context(), CL_MEM_READ_WRITE, particle_buffer, &err);
  if (!positions_buffer || err != CL_SUCCESS) {
    Log::e("Failed to create positions buffer. [%i]", err);
  }

  set_particles(num_particles);

  /*err = clEnqueueWriteBuffer(MCL::device_queue(), input_buffer, CL_TRUE, 0, p_byte_size, &particle_buffer, 0, nullptr, nullptr);
  if (err != CL_SUCCESS) {
    Log::e("Failed to write to input buffer. [%i]", err);
  }*/

  GLuint vs = Program::load_shader("./data/shader/vertex.glsl", GL_VERTEX_SHADER);
  GLuint fs = Program::load_shader("./data/shader/fragment.glsl", GL_FRAGMENT_SHADER);
  
  program.attach_shader(vs);
  program.attach_shader(fs);
  program.link();

  Program::delete_shader(vs);
  Program::delete_shader(fs);

  proj = glm::ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f, -1.f, 1.f);

  glUseProgram(program.id);
  glUniformMatrix4fv(glGetUniformLocation(program.id, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

App::~App() {
  delete kernel;

  clReleaseMemObject(positions_buffer);
  clReleaseMemObject(velocities_buffer);

  glDeleteBuffers(1, &particle_buffer);
  glDeleteVertexArrays(1, &particle_vao);
}

void App::reshape(int width, int height) {
  proj = glm::ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f, -1.f, 1.f);

  glUseProgram(program.id);
  glUniformMatrix4fv(glGetUniformLocation(program.id, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

  this->width = width;
  this->height = height;
}

void App::load() {

}

void App::unload() {

}

void App::update(double delta) {
  if (Keyboard::is_key_pressed(GLFW_KEY_R)) {
    set_particles(num_particles);
  }

  compute();
}

void App::draw(double lerp) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.01f, 0.51f, 0.71f, 1.f);

  glUseProgram(program.id);
  glBindVertexArray(particle_vao);

  glBindBuffer(GL_ARRAY_BUFFER, particle_buffer);
  glDrawArrays(GL_POINTS, 0, num_particles);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
  glUseProgram(0);
}

void App::compute() {
  cl_float2 center = {{
    static_cast<float>(Mouse::get_x()),
    static_cast<float>(Mouse::get_y())
  }};

  cl_float dfactor = Keyboard::is_key_down(GLFW_KEY_LEFT_ALT) ? 1.f : 0.f;

  kernel->set_arg(0, sizeof(cl_mem), &positions_buffer);
  kernel->set_arg(1, sizeof(cl_mem), &velocities_buffer);
  kernel->set_arg(2, sizeof(cl_float2), &center);
  kernel->set_arg(3, sizeof(cl_float), &dfactor);

  if (clEnqueueAcquireGLObjects(MCL::device_queue(), 1, &positions_buffer, 0, nullptr, nullptr)) {
    Log::e("Failed to attach positions buffer");
  }

  kernel->exec(1, num_particles);

  if (clEnqueueReleaseGLObjects(MCL::device_queue(), 1, &positions_buffer, 0, nullptr, nullptr)) {
    Log::e("Failed to release positions buffer");
  }

  clFlush(MCL::device_queue());

  /*float* data;
  int size = num_particles * 2 * sizeof(float);
  if (clEnqueueReadBuffer(MCL::device_queue(), output_buffer, true, 0, size, &data, 0, nullptr, nullptr)) {
    Log::e("fail");
  } else {
    glBindBuffer(GL_ARRAY_BUFFER, particle_buffer);
    glBufferData(GL_ARRAY_BUFFER, size, &data, GL_STATIC_DRAW);    
  }*/

}

void App::set_particles(int num) {
  num_particles = num;
  size_t num_vertices = num_particles * 2;

  std::vector<GLfloat> data(num_vertices);
  std::vector<GLfloat> vel(num_vertices);

  for (int i = 0; i < num_vertices; i += 2) {
    data[i    ] = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * width;// * height + (width-height)/2;
    data[i + 1] = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * height;

    vel[i    ] = 0;
    vel[i + 1] = 0;

    //Log::d("data[%i/%i] = %f, %f", i, i+1, data[i], data[i+1]);
  }
  glBindBuffer(GL_ARRAY_BUFFER, particle_buffer);
  glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);

  int err = clEnqueueWriteBuffer(MCL::device_queue(), positions_buffer, CL_TRUE, 0, sizeof(float) * 2 * num_particles, &data[0], 0, nullptr, nullptr);
  if (err != CL_SUCCESS) {
    Log::e("Failed to write to positions buffer from particle buffer. [%i]", err);
  }

  err = clEnqueueWriteBuffer(MCL::device_queue(), velocities_buffer, CL_TRUE, 0, sizeof(float) * 2 * num_particles, &vel[0], 0, nullptr, nullptr);
  if (err != CL_SUCCESS) {
    Log::e("Failed to write to velocities buffer from particle buffer. [%i]", err);
  }
}

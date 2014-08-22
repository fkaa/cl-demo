#include "app.h"

App::App(int width, int height, int num_particles) :  use_gpu(false), width(width), height(height), num_particles(num_particles) {
  glGenBuffers(1, &particle_buffer);

  set_particles(num_particles);

  GLuint vs = Program::load_shader("./data/shader/vertex.glsl", GL_VERTEX_SHADER);
  GLuint fs = Program::load_shader("./data/shader/fragment.glsl", GL_FRAGMENT_SHADER);
  GLuint cs = Program::load_shader("./data/shader/compute.glsl", GL_COMPUTE_SHADER);
  
  program.attach_shader(vs);
  program.attach_shader(fs);
  program.link();

  compute.attach_shader(cs);
  compute.link();

  Program::delete_shader(vs);
  Program::delete_shader(fs);
  Program::delete_shader(cs);

  proj = glm::ortho(-width / 2, width / 2, -height / 2, height / 2, -1, 1);

  glUseProgram(program.id);
  glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

App::~App() {

}

void App::load() {

}

void App::unload() {

}

void App::update(double delta) {
  att = glm::vec2(Mouse::get_x(), Mouse::get_y());

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particle_buffer);
  glUseProgram(compute.id);
  glUniform2f(0, att.x, att.y);

  glDispatchCompute((num_particles / 256) + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

void App::draw(double lerp) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.f, 0.f, 0.f, 1.f);

  glUseProgram(program.id);
  glBindBuffer(GL_ARRAY_BUFFER, particle_buffer);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glEnableClientState(GL_VERTEX_ARRAY);
  glDrawArrays(GL_POINTS, 0, num_particles);
  glDisableClientState(GL_VERTEX_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void App::set_particles(int num) {
  num_particles = num;

  glUseProgram(compute.id);
  glUniform1i(1, num_particles);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, particle_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER, num * sizeof(Particle), NULL ,GL_STATIC_DRAW);

  struct Particle* particles = (struct Particle*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, num * sizeof(Particle), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

  float r_x, r_y;
  for (size_t i = 0; i < num; i++) {
    r_x = rand() % width;
    r_y = rand() % height;

    particles[i].curr_pos = glm::vec4(r_x, r_y, 1.f, 1.f);
    particles[i].prev_pos = glm::vec4(r_x, r_y, 1.f, 1.f);
  }

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

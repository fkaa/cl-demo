#include "app.h"

App::App(int width, int height, int size)
  : use_gpu(false),
    width(width),
    height(height),
    num_particles(size) {
  glGenBuffers(1, &particle_buffer);

  set_particles(num_particles);

  kernel = new Kernel("./data/kernel/particles.cl", "particle_run");

  GLuint vs = Program::load_shader("./data/shader/vertex.glsl", GL_VERTEX_SHADER);
  GLuint fs = Program::load_shader("./data/shader/fragment.glsl", GL_FRAGMENT_SHADER);
  
  program.attach_shader(vs);
  program.attach_shader(fs);
  program.link();

  Program::delete_shader(vs);
  Program::delete_shader(fs);

  proj = glm::ortho(-width / 2, width / 2, -height / 2, height / 2, -1, 1);

  glUseProgram(program.id);
  glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(proj));

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

App::~App() {
  delete kernel;
}

void App::load() {

}

void App::unload() {

}

void App::update(double delta) {
  att = glm::vec2(Mouse::get_x(), Mouse::get_y());

}

void App::draw(double lerp) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.05f, 0.05f, 0.05f, 1.f);

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


}

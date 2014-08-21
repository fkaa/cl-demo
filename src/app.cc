#include "app.h"

App::App() : use_gpu(false) {
  GLuint vs = Program::load_shader("./data/shader/vs.glsl", GL_VERTEX_SHADER);
  GLuint fs = Program::load_shader("./data/shader/fs.glsl", GL_FRAGMENT_SHADER);
  
  program.attach_shader(vs);
  program.attach_shader(fs);
  program.link();

  Program::delete_shader(vs);
  Program::delete_shader(fs);
}

App::~App() {

}

void App::load() {

}

void App::unload() {

}

void App::update(double delta) {

}

void App::draw(double lerp) {

}

#ifndef APP_H
#define APP_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "compute/kernel.h"
#include "compute/mcl.h"

#include "graphics/program.h"

#include "state/state.h"

#include "particle.h"
#include "input.h"

class App : public State {
 public:
  App(int width, int height, int num_particles);
  virtual ~App();

  virtual void load();
  virtual void unload();
  virtual void update(double delta);
  virtual void draw(double lerp);

  void set_particles(int num);

  bool use_gpu;
 private:
  int width;
  int height;
  int num_particles;
  GLuint particle_buffer;

  Program program;
  Kernel* kernel;

  glm::vec2 att;
  glm::mat4 proj;
};

#endif // APP_H

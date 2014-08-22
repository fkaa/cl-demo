#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

struct Particle {
  glm::vec4 curr_pos;
  glm::vec4 prev_pos;
};

#endif

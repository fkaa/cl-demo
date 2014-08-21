#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "log.h"

class Framebuffer {
 public:
  Framebuffer(int width, int height);
  ~Framebuffer();
  GLuint col_id, id;
  int width, height;
  void resize(int width, int height);
  static void bind(GLuint buf);
  static void read(GLuint buf);
  static void draw(GLuint buf);
  static void blit(Framebuffer* src, int x, int y, int width, int height, int filter);
};

#endif

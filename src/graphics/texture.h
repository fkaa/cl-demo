#ifndef TEXTURE_H
#define TEXTURE_H

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <memory>

#include "log.h"

class Texture;

typedef std::shared_ptr<Texture> TextureHandle;

class Texture {
 public:
  Texture(std::string path);
  ~Texture();
  GLuint id;
  int width, height;
};


#endif

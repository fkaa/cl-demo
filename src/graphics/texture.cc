#include "graphics/texture.h"

Texture::Texture(std::string path) {
  int components;
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &components, 4);
  if (data) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_image_free(data);
    Log::d("Created texture '%s' [id=%i,width=%i,height=%i]", path.c_str(), id, width, height);
  } else {
    Log::e("Failed to load texture '%s'", path.c_str());
  }
}

Texture::~Texture() {
  Log::d("Deleting texture [id=%i,width=%i,height=%i]", id, width, height);
  glDeleteTextures(1, &id);
}

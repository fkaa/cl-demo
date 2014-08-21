#include "graphics/fbo.h"

Framebuffer::Framebuffer(int width, int height) : width(width), height(height) {
  glGenTextures(1, &col_id);
  resize(width, height);

  glGenFramebuffers(1, &id);
  if (!id) {
    Log::e("Failed to create framebuffer.");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, id);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, col_id, 0);
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch (status) {
    case GL_FRAMEBUFFER_COMPLETE:
      Log::v("Created framebuffer [width=%i,height=%i]", width, height);
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      Log::e("Invalid framebuffer, incomplete attachment");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      Log::e("Invalid framebuffer, missing attachment");
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
      Log::e("Invalid framebuffer, unsupported format");
    default:
      Log::e("Invalid framebuffer [error=\033[31m%i\033[m]", status);
      break;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer() {
  Log::d("Deleting framebuffer [width=%i,height=%i]", width, height);
  glDeleteFramebuffers(1, &id);
  glDeleteTextures(1, &col_id);
}

void Framebuffer::resize(int width, int height) {
  Log::v("Resizing framebuffer [width=%i,height=%i]", width, height);
  glBindTexture(GL_TEXTURE_2D, col_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
}

void Framebuffer::blit(Framebuffer* fbo, int x, int y, int width, int height, int filter) {
  glBlitFramebuffer(0, 0, fbo->width, fbo->height, x, y, width, height, GL_COLOR_BUFFER_BIT, filter);
}

void Framebuffer::bind(GLuint buf) {
  glBindFramebuffer(GL_FRAMEBUFFER, buf);
}

void Framebuffer::read(GLuint buf) {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, buf);
}

void Framebuffer::draw(GLuint buf) {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buf);
}

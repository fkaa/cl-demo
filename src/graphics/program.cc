#include "graphics/program.h"

Program::Program() {
  id = glCreateProgram();
}

Program::~Program() {
  glDeleteProgram(id);
}

void Program::attach_shader(GLuint shader) {
  glAttachShader(id, shader);
}

void Program::link() {
  glLinkProgram(id);

  GLint link_status = GL_FALSE, log_len;
  glGetProgramiv(id, GL_LINK_STATUS, &link_status);
  if(glGetError() != GL_NO_ERROR || link_status == GL_FALSE) {
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_len);
    if (log_len > 0) {
      std::vector<char> link_error(log_len);
      glGetProgramInfoLog(id, log_len, NULL, &link_error[0]);
      Log::e("%s", &link_error[0]);
    }
  }
}

GLuint Program::load_shader(const char* path, GLenum type) {
  std::vector<char> data;
  if (FILE* fp = fopen(path, "r")) {
    char buf[1024];
    while (size_t len = fread(buf, 1, sizeof(buf), fp)) {
      data.insert(data.end(), buf, buf + len);
    }
    fclose(fp);
  }
  const char* src = &data[0];

  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  GLint source_status = GL_FALSE, log_len;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &source_status);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
  if (log_len > 0) {
    std::vector<char> shader_error(log_len);
    glGetShaderInfoLog(shader, log_len, NULL, &shader_error[0]);
    Log::e("%s", &shader_error[0]);
  }

  return shader;
}

void Program::delete_shader(GLuint shader) {
  glDeleteShader(shader);
}

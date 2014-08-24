#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "math_helper.h"
#include "asset.h"
#include "log.h"

#include "state/governor.h"
#include "state/state.h"

#include "graphics/texture.h"
#include "graphics/sprite.h"
#include "graphics/fbo.h"

#include "compute/mcl.h"

#include "app.h"
#include "input.h"
#include "timer.h"

#define APP_WIDTH 640
#define APP_HEIGHT 480

namespace {

int wnd_width = APP_WIDTH;
int wnd_height = APP_HEIGHT;

void resize(GLFWwindow* window, int w, int h) {
  wnd_width = w;
  wnd_height = h;
}

} // namespace

int main(int argc, const char *argv[]) {
  Log::d("Initializing glfw");
  if (!glfwInit()) {
    Log::e("Failed to initialize glfw, closing.");
    return 1;
  }

  Log::d("Creating window [width=%i,height=%i,title='untitled']", wnd_width, wnd_height);
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  GLFWwindow* wnd = glfwCreateWindow(wnd_width, wnd_height, "untitled", NULL, NULL);
  if (!wnd) {
    Log::e("Failed to create window, closing.");
    return 1;
  }
  glfwSetFramebufferSizeCallback(wnd, resize);
  glfwMakeContextCurrent(wnd);


  Log::d("Initializing GLEW");
  glewExperimental = GL_TRUE;
  if (GLEW_OK != glewInit()) {
    Log::e("Failed to initialize GLEW, closing.");
    return 1;
  }
  Log::v("OpenGL: %s", glGetString(GL_VERSION));

  glClearColor(0.05f, 0.05f, 0.05f, 1.f);

  if (!MCL::init(true)) {
    Log::e("Failed to initialize OpenCL, closing.");
  }

  Keyboard::init(wnd);
  Mouse::init(wnd);

  Governor::put("main_app", new App(APP_WIDTH, APP_HEIGHT, 1000));
  Governor::set("main_app");

  const double dt = 1. / 60.;
  double time = 0.;

  double current_time = glfwGetTime();
  double accumulator = 0.;

  //  Log::log("foo, %s\n", "bar");

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  while (!glfwWindowShouldClose(wnd)) {
    const double new_time = glfwGetTime();
    const double frame_time = new_time - current_time;
    current_time = new_time;
    accumulator += frame_time;

    while (accumulator >= dt) {
      Timer::update_all(dt);
      Governor::update(dt);
      accumulator -= dt;
      time += dt;

      Mouse::poll();
      Keyboard::poll();
      glfwPollEvents();
    }
    const double lerp = accumulator / dt;

    Governor::draw(lerp);

    glfwSwapBuffers(wnd);
  }
  Log::d("Closing program.");

  Asset::clear();
  Governor::destroy();
  glfwDestroyWindow(wnd);
  return 0;
}

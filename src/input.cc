#include "input.h"

std::map<int, bool> Keyboard::press_keys;
std::map<int, bool> Keyboard::release_keys;
std::map<int, bool> Keyboard::repeat_keys;

void Keyboard::key_cb(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    press_keys[key] = true;
    repeat_keys[key] = true;
  } else if (action == GLFW_RELEASE) {
    release_keys[key] = true;
    repeat_keys[key] = false;
  }
}

void Keyboard::init(GLFWwindow* wnd) {
  glfwSetKeyCallback(wnd, key_cb);
}

void Keyboard::poll() {
  press_keys.clear();
  release_keys.clear();
}

bool Keyboard::is_key_pressed(int key) {
  return press_keys.count(key) > 0;
}


bool Keyboard::is_key_down(int key) {
  return repeat_keys[key];
}

bool Keyboard::is_key_released(int key) {
  return release_keys.count(key) > 0;
}

std::map<int, bool> Mouse::press_btns;
std::map<int, bool> Mouse::release_btns;
std::map<int, bool> Mouse::repeat_btns;

int Mouse::m_x;
int Mouse::m_y;

void Mouse::cursor_cb(GLFWwindow* wnd, double x, double y) {
  m_x = (int)x;
  m_y = (int)y;
}

void Mouse::mouse_cb(GLFWwindow* wnd, int button, int action, int mods) {
  if (action == GLFW_PRESS) {
    press_btns[button] = true;
    repeat_btns[button] = true;
  } else if (action == GLFW_RELEASE) {
    release_btns[button] = true;
    repeat_btns[button] = false;
  }
}

void Mouse::init(GLFWwindow* wnd) {
  glfwSetCursorPosCallback(wnd, cursor_cb);
  glfwSetMouseButtonCallback(wnd, mouse_cb);
}

void Mouse::poll() {
  press_btns.clear();
  release_btns.clear();
}

bool Mouse::is_btn_pressed(int btn) {
  return press_btns.count(btn) > 0;
}

bool Mouse::is_btn_down(int btn) {
  return repeat_btns[btn];
}

bool Mouse::is_btn_released(int btn) {
  return release_btns.count(btn) > 0;
}

int Mouse::get_x() {
  return m_x;
}

int Mouse::get_y() {
  return m_y;
}

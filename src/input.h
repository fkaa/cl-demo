#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>

#include "log.h"

class Keyboard {
 public:
  static void init(GLFWwindow* wnd);
  static void poll();

  static bool is_key_down(int key);
  static bool is_key_pressed(int key);
  static bool is_key_released(int key);
 private:
  Keyboard() {}
  ~Keyboard() {}
  
  static void key_cb(GLFWwindow* wnd, int key, int scancode, int action, int mods);
  static std::map<int, bool> press_keys;
  static std::map<int, bool> release_keys;
  static std::map<int, bool> repeat_keys;
};

class Mouse {
 public:
  static void init(GLFWwindow* wnd);
  static void poll();

  static bool is_btn_down(int btn);
  static bool is_btn_pressed(int btn);
  static bool is_btn_released(int btn);
  static double get_x();
  static double get_y();
  /*bool is_button_down(int key);
  bool is_button_pressed(int key);
  bool is_button_released(int key);*/
 private:
  Mouse() {}
  ~Mouse() {}

  static void cursor_cb(GLFWwindow* wnd, double x, double y);
  static void mouse_cb(GLFWwindow* wnd, int button, int action, int mods);
  static double m_x, m_y;
  static std::map<int, bool> press_btns;
  static std::map<int, bool> release_btns;
  static std::map<int, bool> repeat_btns;
};

#endif

#ifndef LOGGER_H
#define LOGGER_H

#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>

namespace Log {

template <typename... Args>
static void p(const char* label, const char* text, Args&&... args) {
  time_t t = time(0);
  struct tm* now = localtime(&t);

  std::stringstream ss;
  ss << "\033[33m";
  ss << std::setfill('0') << std::setw(2) << now->tm_hour << ":";
  ss << std::setfill('0') << std::setw(2) << now->tm_min << ":";
  ss << std::setfill('0') << std::setw(2) << now->tm_sec;
  ss << " \033[32m[" << label << "\033[32m]\033[m " << text << std::endl;
  printf(ss.str().c_str(), args...);
}

template <typename... Args>
static void d(const char* text, Args&&... args) {
  p("\033[33mDEBUG  ", text, args...);
}

template <typename... Args>
static void e(const char* text, Args&&... args) {
  p("\033[31mERROR  ", text, args...);
}

template <typename... Args>
static void v(const char* text, Args&&... args) {
  p("VERBOSE", text, args...);
}


} // namespace Log

#endif // LOGGER_H

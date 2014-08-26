#ifndef TIMER_H
#define TIMER_H

#include <set>

class Timer {
 public:
  explicit Timer(float len) : expire_time(len) { timers.insert(this); }
  ~Timer() { timers.erase(this); }
  bool active() const { return current_time < expire_time; }
  bool expired() const { return !active(); }
  float elapsed_time() { return current_time; }
  void reset() { current_time = 0; }

  static void update_all(float delta);
  static float time;
 private:
  void update(float delta) { if (active()) { current_time += delta; } }
  float current_time;
  float expire_time;

  static std::set<Timer*> timers;
};

#endif // TIMER_H

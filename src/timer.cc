#include "timer.h"

std::set<Timer*> Timer::timers;
float Timer::time = 0.f;

void Timer::update_all(float delta) {
  for (std::set<Timer*>::iterator iter = timers.begin(); iter != timers.end(); iter++) {
    (*iter)->update(delta);
  }
}

#include "governor.h"

namespace Governor {

namespace {

std::map<const char*, State*> state_map;
std::deque<State*> states;

} // namespace

bool set(const char* id) {
  //if (state_map.count(id) > 0) return false;

  State* state = state_map[id];
  states.push_front(state);
  return true;
}

bool push(const char* id) {
  //if (state_map.count(id) > 0) return false;

  State* state = state_map[id];
  states.push_front(state);
  return true;
}

void pop() {
  states.pop_front();
}

void update(double delta) {
  for (std::deque<State*>::iterator it = states.begin(); it != states.end(); ++it) {
    if (!(*it)->paused) (*it)->update(delta);
  }
}

void draw(double lerp) {
  glClear(GL_COLOR_BUFFER_BIT);

  for (std::deque<State*>::iterator it = states.begin(); it != states.end(); ++it) {
    (*it)->draw(lerp);
  }
}

void put(const char* id, State* state) {
  state_map.insert(std::pair<const char*, State*>(id, state));
}

void destroy() {
  for (int i = 0; i < states.size(); i++) {
    delete states[i];
  }
  auto iter = state_map.begin();
  while (iter != state_map.end()) {
    state_map.erase(iter++);
  }
  
  state_map.clear();
  states.clear();

}

} // namespace Governor

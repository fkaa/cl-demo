#ifndef GOVERNOR_H
#define GOVERNOR_H

#include <GLFW/glfw3.h>
#include <deque>
#include <map>
#include "state.h"

namespace Governor {

bool set(const char* id);
bool push(const char* id);
void pop();

void update(double delta);
void draw(double lerp);

void put(const char* id, State* state);
void destroy();

} // namespace Governor

#endif // GOVERNOR_H

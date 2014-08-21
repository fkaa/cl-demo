#ifndef APP_H
#define APP_H

#include "graphics/program.h"

#include "state/state.h"

class App : public State {
 public:
  App();
  virtual ~App();

  virtual void load();
  virtual void unload();
  virtual void update(double delta);
  virtual void draw(double lerp);

  bool use_gpu;
 private:
  Program program;
};

#endif // APP_H

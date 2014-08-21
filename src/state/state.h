#ifndef STATE_H
#define STATE_H

class State {
 public:
  State() : paused(false) {};
  virtual ~State() {};
  
  // state lifecycle
  virtual void load() = 0;
  virtual void unload() {}
  virtual void update(double delta) {}
  virtual void draw(double lerp) {}
  
  // state input
  virtual void on_key() {}
  virtual void on_mouse() {}

  bool paused;
};

#endif // STATE_H

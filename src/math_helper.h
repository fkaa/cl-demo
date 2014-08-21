#ifndef MATH_HELPER_H
#define MATH_HELPER_H

template<class T>
class MathHelper {
 public:
  static inline T lerp(T v0, T v1, float t) {
    return v0 * (1 - t) + v1 * t;
  }
 private:
  MathHelper();
  ~MathHelper();
};

#endif // MATH_HELPER_H

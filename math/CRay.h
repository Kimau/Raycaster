#pragma once
#include "../types.h"
#include "CVector3.h"

struct ray3 {
public:
  ray3() : a(vec3(0, 0, 0)), b(vec3(0, 0, 1)) {}
  ray3(const ray3 &r) : a(r.a), b(r.b) {}
  ray3(const vec3 &origin, const vec3 &dir) : a(origin), b(dir) {}

  inline vec3 PointAt(float t) const { return a + b * t; }

  inline const vec3 &origin() const { return a; }
  inline const vec3 &dir() const { return b; }

  vec3 a, b;
};

#pragma once
#include "../types.h"
#include "CVector3.h"

struct ray {
public:
  ray() : a(vec3(0, 0, 0)), b(vec3(0, 0, 1)) {}
  ray(const ray &r) : a(r.a), b(r.b) {}
  ray(const vec3 &origin, const vec3 &dir) : a(origin), b(dir) {}

  inline vec3 PointAt(float t) const { return a + b * t; }

  inline const vec3 &origin() const { return a; }
  inline const vec3 &dir() const { return b; }

  vec3 a, b;
};

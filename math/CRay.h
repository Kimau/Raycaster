#pragma once

#include "CVector3.h"
#include <cstdint>

class Ray {
public:
  Ray() : a(Vec3(0, 0, 0)), b(Vec3(0, 0, 1)) {}
  Ray(const Ray &r) : a(r.a), b(r.b) {}
  Ray(const Vec3 &origin, const Vec3 &dir) : a(origin), b(dir) {}

  inline Vec3 PointAt(float t) const { return a + b * t; }

  inline const Vec3 &origin() const { return a; }
  inline const Vec3 &dir() const { return b; }

  Vec3 a, b;
};

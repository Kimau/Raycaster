#pragma once
#include "../types.h"

struct vec2 {
public:
	vec2() : x(0.0f), y(0.0f) {}
	vec2(float _X, float _Y) : x(_X), y(_Y){}
	vec2(double _X, double _Y) : x(float(_X)), y(float(_Y)) {}
	vec2(int _X, int _Y) : x(float(_X)), y(float(_Y)) {}
	vec2(const vec2 &v) : x(v.x), y(v.y) {}

  //	Operators
  inline vec2 operator+(vec2 v) const { return vec2(x + v.x, y + v.y); }
  inline vec2 operator-(vec2 v) const { return vec2(x - v.x, y - v.y); }
  inline vec2 operator*(float s) const { return vec2(x * s, y*s); }
  inline vec2 operator/(float s) const { return vec2(x / s, y / s); }

  //	Self Operators
  void operator+=(vec2 _vector);
  void operator-=(vec2 _vector);
  void operator*=(float _num);
  void operator/=(float _num);
  void operator/=(vec2 _vector);

  //	Comparison Operators
  bool operator==(float _mag) const;
  bool operator==(vec2 _vector) const;
  bool operator!=(vec2 _vector) const;

  void Normalize();
  void Invert();
  vec2 getNormalized() const;
  float Magnitude() const;
  float Angle(vec2 _vector) const;

  float x, y;
};

constexpr float dot(const vec2& a, const vec2& b) { return (a.x * b.x) + (a.y * b.y); }


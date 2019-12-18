#pragma once
#include "../types.h"
#include "Cvector2.h"

struct vec3 {
public:
  //	Constructors
	vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	vec3(float _X, float _Y, float _Z) : x(_X), y(_Y), z(_Z) {}
	vec3(double _X, double _Y, double _Z) : x(float(_X)), y(float(_Y)), z(float(_Z)) {}
	vec3(int _X, int _Y, int _Z) : x(float(_X)), y(float(_Y)), z(float(_Z)) {}
	vec3(const vec3 &v) : x(v.x), y(v.y), z(v.z) {}

  //	Operators
  inline vec3 operator+(vec3 v) const { return vec3(x + v.x, y + v.y, z + v.z); }
  inline vec3 operator-(vec3 v) const { return vec3(x - v.x, y - v.y, z - v.z); }
  inline vec3 operator*(float s) const { return vec3(x * s, y*s, z*s); }
  inline vec3 operator/(float s) const { return vec3(x / s, y / s, z / s); }

  inline vec3 operator*(vec3 v) const {
	  return vec3(
		  (y * v.z) - (z * v.y),
		  (z * v.x) - (x * v.z),
		  (x * v.y) - (y * v.x));
  }

  //	Self Operators
  inline void operator+=(vec3 v) { x += v.x; y += v.y; z += v.z; }
  inline void operator-=(vec3 v) { x -= v.x; y -= v.y; z -= v.z; }
  inline void operator*=(float s) { x *= s; y *= s; z *= s; }
  inline void operator/=(float s) { x /= s; y /= s; z /= s; }
  inline void operator*=(vec3 _vector) {
	  x = ((y * _vector.z) - (z * _vector.y));
	  y = ((z * _vector.x) - (x * _vector.z));
	  z = ((x * _vector.y) - (y * _vector.x));
  }

  //	Functions
  void RotateX(float _degree);
  void RotateY(float _degree);
  void RotateZ(float _degree);
  void Rotate(float _degree, vec3 _axis);
  void Normalize();
  void Invert();

  //	Accesors
  vec2 xy() const { return vec2(x, y); }
  vec3 getNormalized() const;
  vec3 getLongitude() const;
  vec3 getLatitude() const;
  float Magnitude() const;
  float SqrdMag() const;
  float Dot(vec3 _vector) const;
  float Angle(vec3 _vector) const;
  float Distance(vec3 _point, vec3 _orig = vec3(0, 0, 0)) const;
  float DistanceFast(vec3 _point) const;
  void getArray(float *_array) const;

  float x, y, z;
};

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
  inline vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
  inline vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
  inline vec3 operator*(float s) const { return vec3(x * s, y*s, z*s); }
  inline vec3 operator/(float s) const { return vec3(x / s, y / s, z / s); }

  inline vec3 operator*(const vec3& v) const {
	  return vec3(
		  (y * v.z) - (z * v.y),
		  (z * v.x) - (x * v.z),
		  (x * v.y) - (y * v.x));
  }

  //	Self Operators
  inline void operator+=(const vec3& v) { x += v.x; y += v.y; z += v.z; }
  inline void operator-=(const vec3& v) { x -= v.x; y -= v.y; z -= v.z; }
  inline void operator*=(float s) { x *= s; y *= s; z *= s; }
  inline void operator/=(float s) { x /= s; y /= s; z /= s; }

  inline void operator*=(const vec3& _vector) {
	  x = ((y * _vector.z) - (z * _vector.y));
	  y = ((z * _vector.x) - (x * _vector.z));
	  z = ((x * _vector.y) - (y * _vector.x));
  }

  //	Functions
  void Normalize();
  inline void Invert() {
	  x = -x;
	  y = -y;
	  z = -z;
  }

  //	Accesors
  inline vec2 xy() const { return vec2(x, y); }
  vec3 getNormalized() const;
  vec3 getLongitude() const;
  vec3 getLatitude() const;
  vec3 RotateX(float _degree) const;
  vec3 RotateY(float _degree) const;
  vec3 RotateZ(float _degree) const;
  vec3 Rotate(float _degree, const vec3& _axis) const;

  float Magnitude() const;
  constexpr inline float SqrdMag() const { return x * x + y * y + z * z; }
  float Angle(const vec3& _vector) const;

  inline float Distance(const vec3& _point, const vec3& _orig = vec3(0, 0, 0)) const {
	  vec3 _top = ((_point - _orig) * (*this));
	  _top /= Magnitude();
	  return _top.Magnitude();
  }

  inline float DistanceFast(const vec3& _point) const {
	 
		  //	Gets the distance to the closest point on the line
		  //	ASSUMES (this) is from Origin and length 1
		  vec3 _top = (_point * (*this));
		  return _top.Magnitude();
  }

  void getArray(float *_array) const {
		  _array[0] = x;
		  _array[1] = y;
		  _array[2] = z;
  }

  float x, y, z;
};

constexpr float dot(const vec3& a, const vec3& b) { return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z)); }
inline vec3 reflect(const vec3& v, const vec3& n) { return v - n * (2.0f * dot(v,n)); }
bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& out);

extern const vec3 up_vec;

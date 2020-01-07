#pragma once
#include "../types.h"
#include "Cvector3.h"

#define SIMD
#ifdef SIMD
#include <xmmintrin.h>
#endif

#ifdef SIMD
struct vec4 {
public:
	//	Constructors
	vec4() {}
	vec4(float _X, float _Y, float _Z, float _W) : x(_X), y(_Y), z(_Z), w(_W) {}
	vec4(double _X, double _Y, double _Z, double _W) : x(float(_X)), y(float(_Y)), z(float(_Z)), w(float(_W)) {}
	vec4(int _X, int _Y, int _Z, int _W) : x(float(_X)), y(float(_Y)), z(float(_Z)), w(float(_W)) {}
	vec4(const vec3& v, float t) : x(v.x), y(v.y), z(v.z), w(t) {}
	vec4(const vec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	//	Operators
	inline vec4 operator+(vec4 v) const { return vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
	inline vec4 operator-(vec4 v) const { return vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
	inline vec4 operator*(float s) const { return vec4(x * s, y*s, z*s, w*s); }
	inline vec4 operator/(float s) const { return vec4(x / s, y / s, z / s, w / s); }

	//	Self Operators
	inline void operator+=(vec4 v) { x += v.x; y += v.y; z += v.z; w += v.w; }
	inline void operator-=(vec4 v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; }
	inline void operator*=(float s) { x *= s; y *= s; z *= s; w *= s; }
	inline void operator/=(float s) { x /= s; y /= s; z /= s; w /= s; }

	//	Accesors
	inline vec3 pt() const { return vec3(x, y, z)*w; }
	inline vec3 xyz() const { return vec3(x, y, z); }

	union 
	{
		__m128 v;
		struct {
			float x, y, z, w;
		};
	};
};
#else 
struct vec4 {
public:
	//	Constructors
	vec4() {}
	vec4(float _X, float _Y, float _Z, float _W) : x(_X), y(_Y), z(_Z), w(_W) {}
	vec4(double _X, double _Y, double _Z, double _W) : x(float(_X)), y(float(_Y)), z(float(_Z)), w(float(_W)) {}
	vec4(int _X, int _Y, int _Z, int _W) : x(float(_X)), y(float(_Y)), z(float(_Z)), w(float(_W)) {}
	vec4(const vec3& v, float t) : x(v.x), y(v.y), z(v.z), w(t) {}
	vec4(const vec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	//	Operators
	inline vec4 operator+(vec4 v) const { return vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
	inline vec4 operator-(vec4 v) const { return vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
	inline vec4 operator*(float s) const { return vec4(x * s, y*s, z*s, w*s); }
	inline vec4 operator/(float s) const { return vec4(x / s, y / s, z / s, w / s); }

	//	Self Operators
	inline void operator+=(vec4 v) { x += v.x; y += v.y; z += v.z; w += v.w; }
	inline void operator-=(vec4 v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; }
	inline void operator*=(float s) { x *= s; y *= s; z *= s; w *= s; }
	inline void operator/=(float s) { x /= s; y /= s; z /= s; w /= s; }

	//	Accesors
	inline vec3 pt() const { return vec3(x, y, z)*w; }
	inline vec3 xyz() const { return vec3(x, y, z); }

	float x, y, z, w;
};
#endif
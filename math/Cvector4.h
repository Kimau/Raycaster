#pragma once
#include "../types.h"
#include "Cvector3.h"

struct vec4 {
public:
	//	Constructors
	vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	vec4(float _X, float _Y, float _Z, float _W) : x(_X), y(_Y), z(_Z), w(_W) {}
	vec4(double _X, double _Y, double _Z, double _W) : x(float(_X)), y(float(_Y)), z(float(_Z)), w(float(_W)) {}
	vec4(int _X, int _Y, int _Z, int _W) : x(float(_X)), y(float(_Y)), z(float(_Z)), w(float(_W)) {}
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

	//	Functions
// 	void RotateX(float _degree);
// 	void RotateY(float _degree);
// 	void RotateZ(float _degree);
// 	void Rotate(float _degree, vec3 _axis);
// 	void Normalize();
// 	void Invert();

	//	Accesors
	inline vec3 xyz() const { return vec3(x, y, z); }
// 	vec3 getLongitude() const;
// 	vec3 getLatitude() const;
// 	float Magnitude() const;
// 	float SqrdMag() const;
// 	float Dot(vec3 _vector) const;
// 	float Angle(vec3 _vector) const;
// 	float Distance(vec3 _point, vec3 _orig = vec3(0, 0, 0)) const;
// 	float DistanceFast(vec3 _point) const;
// 	void getArray(float *_array) const;

	float x, y, z, w;
};


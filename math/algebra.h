#pragma once

#include "../types.h"
#include <math.h>
#include "Cvector2.h"
#include "CVector3.h"
#include "Cvector4.h"
#include "CRay.h"

inline float saturate(const float& t) { if (t > 1.0f)return 1.0f; if (t < 0.0f) return 0.0f; return t; }
inline float lerp(const float& a, const float& b, float t) { return a * (1.0f - t) + b * t; }
inline vec2 lerp(const vec2& a, const vec2& b, float t) { return a * (1.0f - t) + b * t; }
inline vec3 lerp(const vec3& a, const vec3& b, float t) { return a * (1.0f - t) + b * t; }
inline vec4 lerp(const vec4& a, const vec4& b, float t) { return a * (1.0f - t) + b * t; }

inline vec4 createPlaneFromPoints(const vec3& a, const vec3& b, const vec3& c) {
	vec3 norm = ((a - b)*(b - c));
	norm.Normalize();
	return vec4(norm.x, norm.y, norm.z, -norm.Dot(a));
}

inline vec4 createPlaneFromPointNormal(const vec3& pt, const vec3& norm) { return vec4(norm.x, norm.y, norm.z, -norm.Dot(pt)); }
inline float distanceFromPlane(const vec3& pt, const vec4& plane) { return -plane.xyz().Dot(pt) - plane.w; }

inline bool hitPlane(const vec4& p, const vec3& a, const vec3& b) { return (distanceFromPlane(a, p) > 0) != (distanceFromPlane(b, p) > 0); }

inline float intersectPlane(vec4 p, ray r) {
	vec3 dirnorm = r.dir().getNormalized();
	float denom = p.xyz().Dot(dirnorm);

	//	Line is axis of plane
	if (denom == 0.0f)
		return 0.0f;

	// Place both in same frame
	float n = p.w +
		p.x + r.a.x +
		p.y + r.a.y +
		p.z + r.a.z;

	//	Get Distance to intersect
	return n / denom;
}
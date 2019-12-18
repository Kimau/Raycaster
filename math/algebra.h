#pragma once

#include "../types.h"
#include <math.h>
#include "Cvector2.h"
#include "CVector3.h"
#include "Cvector4.h"
#include "CRay.h"

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


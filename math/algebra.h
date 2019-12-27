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

vec4 createPlaneFromPoints(const vec3& a, const vec3& b, const vec3& c);

vec4 createPlaneFromPointNormal(const vec3& pt, const vec3& norm);
float distanceFromPlane(const vec3& pt, const vec4& plane);

vec2 poissonDisc8(int i);
vec3 poissonSphere8(int i);
vec2 poissonDisc50(int i);
vec3 poissonSphere50(int i);


float random_float();
vec3 random_in_unit_sphere();

#include "algebra.h"


vec4 createPlaneFromPoints(const vec3& a, const vec3& b, const vec3& c) {
	vec3 norm = ((a - b)*(b - c));
	norm.Normalize();
	return vec4(norm.x, norm.y, norm.z, -norm.Dot(a));
}

vec4 createPlaneFromPointNormal(const vec3& pt, const vec3& norm) { return vec4(norm.x, norm.y, norm.z, -norm.Dot(pt)); }
float distanceFromPlane(const vec3& pt, const vec4& plane) { return -plane.xyz().Dot(pt) - plane.w; }

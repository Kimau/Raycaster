#include "algebra.h"


vec4 createPlaneFromPoints(const vec3& a, const vec3& b, const vec3& c) {
	vec3 norm = ((a - b)*(b - c));
	norm.Normalize();
	return vec4(norm.x, norm.y, norm.z, -norm.Dot(a));
}

vec4 createPlaneFromPointNormal(const vec3& pt, const vec3& norm) { return vec4(norm.x, norm.y, norm.z, -norm.Dot(pt)); }
float distanceFromPlane(const vec3& pt, const vec4& plane) { return -plane.xyz().Dot(pt) - plane.w; }

bool hitPlane(const vec4& p, const vec3& a, const vec3& b) { return (distanceFromPlane(a, p) > 0) != (distanceFromPlane(b, p) > 0); }

float intersectPlane(vec4 p, ray r) {
	float denom = p.xyz().Dot(r.dir().getNormalized());

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

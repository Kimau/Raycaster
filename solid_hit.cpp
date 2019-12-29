#include "solid_hit.h"

bool solid_ball::hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const
{
	const vec3 tp = v.xyz();
	const vec3 oc = r.origin() - tp;
	const float oc_mag_sq = oc.SqrdMag();
	if (oc_mag_sq < (v.w*v.w)) {
		float oc_mag = sqrtf(oc_mag_sq);
		float t = v.w - oc_mag;
		if ((t < tmin) || (t > tmax)) return false;

		if (hit_data != nullptr) {
			hit_data->t = v.w - oc_mag;
			hit_data->p = r.PointAt(hit_data->t);
			hit_data->n = (hit_data->p - tp) / v.w;
		}
		return true;
	}

	float a = dot(r.b, r.b);
	float b = 2.0f * dot(oc, r.b);
	float c = dot(oc, oc) - v.w*v.w;
	float discriminant = b * b - 4.0f*a*c;

	if (discriminant < 0)
		return false;

	float t = (-b - sqrtf(discriminant)) / (2.0f*a);
	if ((t < tmin) || (t > tmax)) return false;

	if (hit_data != nullptr) {
		hit_data->t = t;
		hit_data->p = r.PointAt(hit_data->t);
		hit_data->n = (hit_data->p - tp) / v.w;
	}

	return true;
}

bool solid_skysphere::hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const
{
	if (hit_data != nullptr) {
		const vec3 oc = r.origin();
		float oc_mag_sq = oc.SqrdMag();

		float radius = tmax + sqrtf(oc_mag_sq);
		hit_data->p = r.PointAt(tmax);
		hit_data->n = hit_data->p / radius;
		hit_data->t = tmax;
	}

	return true;
}

bool solid_plane::hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const
{
	const vec3 pnorm = v.xyz();
	float denom = dot(pnorm, r.b);

	if (fabsf(denom) <= 0.000001f) return false;

	const vec3 plane0 = pnorm * v.w;
	const vec3 p0l0 = plane0 - r.origin();
	const float t = dot(p0l0, pnorm) / denom;

	if ((t < tmin) || (t > tmax)) return false;

	if (hit_data != nullptr) {
		hit_data->t = t;
		hit_data->p = r.PointAt(hit_data->t);
		hit_data->n = pnorm;
	}

	return true;
}

bool solid_disc::hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const
{
	const vec3 pnorm = v.xyz();
	float denom = dot(pnorm, r.b);

	if (fabsf(denom) <= 0.000001f) return false;

	const vec3 plane0 = v.xyz() * v.w;
	const vec3 p0l0 = plane0 - r.origin();
	const float t = dot(p0l0, pnorm) / denom;

	if ((t < tmin) || (t > tmax)) return false;

	const vec3 p = r.PointAt(t);
	const float dist2 = (p - plane0).SqrdMag();

	if (dist2 > r2) return false;

	if (hit_data != nullptr) {

		hit_data->t = t;
		hit_data->p = p;
		hit_data->n = pnorm;
	}

	return true;
}

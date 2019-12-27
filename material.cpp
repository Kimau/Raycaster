#include "material.h"



bool mat_flat::colour(ray3& ray, const hit_record& hit, vec3& out_colour) const
{
	out_colour = rgb;
	return true;
}

bool mat_normal::colour(ray3& ray, const hit_record& hit, vec3& out_colour) const
{
	out_colour = hit.n * 0.5f + vec3(0.5f, 0.5f, 0.5f);
	return true;
}

bool mat_diffuse::colour(ray3& ray, const hit_record& hit, vec3& out_colour) const
{
	ray.a = hit.p;
	ray.b = reflect(ray.b, hit.n);

	if (r > 0.0f)
		ray.b += poissonSphere8(rand() % 8) * saturate(r);

	return false;
}

bool mat_lambertian::colour(ray3& ray, const hit_record& hit, vec3& out_colour) const
{
	ray.a = hit.p;
	vec3 o = reflect(ray.b, hit.n);

	if (r > 0.0f)
		o += poissonSphere8(rand() % 8) * saturate(r);

	if (dot(ray.b, o) < 0.0f) {
		out_colour = vec3(0.0f, 0.0f, 0.0f);
		return true;
	}

	ray.b = o;
	return false;
}

bool mat_glass::colour(ray3& ray, const hit_record& hit, vec3& out_colour) const
{
	ray.a = hit.p;
	vec3 out;
	if (refract(ray.b, hit.n, 0.0f, out)) {
		ray.b = out + poissonSphere8(rand() % 8) * saturate(r);
		return false;
	}

	return true;
}

bool mat_tiles::colour(ray3& ray, const hit_record& hit, vec3& out_colour) const
{
	if ((sinf(hit.p.x) > 0) ^ (cosf(hit.p.y) > 0)) {
		ray.a = hit.p;
	}
	else {
		ray.a = hit.p;
		ray.b = reflect(ray.b, hit.n);
	}

	if (r > 0.0f)
		ray.b += poissonSphere8(rand() % 8) * saturate(r);

	return false;
}

bool mat_sky::colour(ray3& ray, const hit_record& hit, vec3& out_colour) const
{
	// Proper method


	int i = 0;
	while ((i < num_cols) && (cols[i].w < hit.n.z)) ++i;

	if (i == 0) out_colour = cols[0].xyz();
	else if (i >= num_cols) out_colour = cols[num_cols - 1].xyz();
	else out_colour = lerp(cols[i - 1].xyz(), cols[i].xyz(), (hit.n.z - cols[i - 1].w) / (cols[i].w - cols[i - 1].w));

	return true;
}

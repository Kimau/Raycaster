#include "material.h"

bool mat_skysphere::scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const
{
	int i = 0;
	while ((i < num_cols) && (cols[i].w < hit.n.z)) ++i;
	
	if (i == 0) col = cols[0].xyz();
	else if (i >= num_cols) col = cols[num_cols - 1].xyz();
	else col = lerp(cols[i - 1].xyz(), cols[i].xyz(), (hit.n.z - cols[i - 1].w) / (cols[i].w - cols[i - 1].w));

	return false;
}

bool mat_normal::scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const
{
	col = hit.n*0.5f + vec3(0.5f, 0.5f, 0.5f);
	return false;
}

bool mat_flat_colour::scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const
{
	col = rgb;
	return false;
}

bool mat_tiles::scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const
{
	scattered = ray3(hit.p, reflect(r.b, hit.n));
	
	// if(roughness > 0.0f) scattered.b += random_in_unit_sphere()*roughness;		

	if ((sinf(hit.p.x * 3.0f) > 0) != (sinf(hit.p.y * 3.0f) > 0))
		col = a;
	else
		col = b;
	return true;
}

bool mat_lambertian::scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const
{
	scattered = ray3(hit.p, hit.n);	
	if (roughness > 0.0f) scattered.b += random_in_unit_sphere()*roughness;

	col = albedo;
	return true;
}

bool mat_diffuse::scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const
{
	scattered = ray3(hit.p, reflect(r.b, hit.n));
	if (roughness > 0.0f) scattered.b += random_in_unit_sphere()*roughness;

	col = albedo;
	return true;
}


bool mat_metal::scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const
{
	vec3 reflected = reflect(r.b.getNormalized(), hit.n);
	scattered = ray3(hit.p, reflected + random_in_unit_sphere() * roughness);
	col = albedo;
	return (dot(scattered.dir(), hit.n) > 0);
}

bool mat_dielectric::scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const
{
	vec3 outward_normal = hit.n;
	float ni_over_nt;
	col = vec3(1.0f, 1.0f, 1.0f);
	float cosine = dot(r.b, hit.n) / r.b.Magnitude();
	if (dot(r.b, hit.n) > 0) {
		outward_normal *= -1.0f;
		ni_over_nt = ref_idx;
		cosine = sqrt(1 - ref_idx * ref_idx*(1 - cosine * cosine));
	}
	else {
		ni_over_nt = 1.0f / ref_idx;
		cosine *= -1.0f;
	}

	vec3 refracted;
	if (refract(r.b, outward_normal, ni_over_nt, refracted) && (random_float() < schlick(cosine, ref_idx)))
		scattered = ray3(hit.p, reflect(r.b, hit.n));
	else
		scattered = ray3(hit.p, refracted);

	return true;
}


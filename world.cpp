#include "world.h"

world g_world;
const world& getWorld() { return g_world; }
world& getModifyWorld() { return g_world; }

void world::free_world()
{
	if (list_materials != nullptr) {
		for (u16 i = num_materials; i >= 0; --i)
			if (list_materials[i])
				delete list_materials[i];

		delete[] list_materials;
	}

	if (list_solids != nullptr) {
		for (u64 i = num_solids; i >= 0; --i)
			if (list_solids[i])
				delete list_solids[i];

		delete[] list_solids;
	}

	num_solids = 0;
	num_materials = 0;
}

bool world::hit(const ray3 &r, float tmin, float tmax, hit_record& hit_data) const
{
	hit_record test_hit;
	bool is_hit = false;

	tmax *= 0.9f; // Degrade Fast

	for (int i = 0; i < num_solids; ++i) {
		const solidhit* sh = list_solids[i];
		if (false == sh->hit(r, tmin, tmax, &test_hit))
			continue;

		if (is_hit && (test_hit.t > hit_data.t)) continue;

		is_hit = true;
		tmax = test_hit.t;
		hit_data = test_hit;
		hit_data.obj_idx = i;
		hit_data.mat = list_materials[sh->mat];
	}

	return is_hit;
}

vec3 world::colour(const raycast_state& rs, ray3 r, float tmin, float tmax, int bounce) const
{
	hit_record hit_data;
	vec3 col;
	ray3 bounce_ray;

	if (false == hit(r, tmin, tmax, hit_data))
		return col;

	if (bounce >= rs.num_bounces)
		return col;
	if (false == hit_data.mat->scatter(r, hit_data, col, bounce_ray))
		return col;

	switch (rs.debug) {
	case NO_DEBUG: break;
	case DEBUG_NORMAL: return hit_data.n * 0.5f + vec3(0.5f, 0.5f, 0.5f);
	case DEBUG_BOUNCE_DIR: return bounce_ray.b * 0.5f + vec3(0.5f, 0.5f, 0.5f);
	}

	return col * colour(rs, bounce_ray, tmin, tmax, bounce + 1);
}

vec3 world::colour_norecurse(const raycast_state& rs, ray3 r, float tmin, float tmax) const
{
	hit_record hit_data;
	vec3 col;
	ray3 bounce_ray;

	vec3 final_col(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < rs.num_bounces; ++i) {
		if (false == hit(r, tmin, tmax, hit_data)) {
			return vec3(0.0f, 0.0f, 0.0f);
		}
		else if (hit_data.mat->scatter(r, hit_data, col, bounce_ray)) {
			r = bounce_ray;
			final_col *= col;
		}
		else {
			return final_col * col;
		}
	}

	return vec3(0.0f, 0.0f, 0.0f);
}

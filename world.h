#pragma once
#include "core.h"
#include "raycast.h"

#include "hit_record.h"
#include "material.h"
#include "solid_hit.h"

class world {
public:
	material** list_materials = nullptr;
	solidhit** list_solids = nullptr;
	u64 num_solids = 0;
	u16 num_materials = 0;

	void free_world();

	bool hit(const ray3 &r, float tmin, float tmax, hit_record& hit_data) const;
	vec3 colour(const raycast_state& rs, ray3 r, float tmin, float tmax, int bounce) const;
	vec3 colour_norecurse(const raycast_state& rs, ray3 r, float tmin, float tmax) const;
};

const world& getWorld();
world& getModifyWorld();

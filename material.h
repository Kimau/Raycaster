#pragma once
#include "hit_record.h"
#include <vector>



class material {
public:
	virtual bool scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const = 0;
};


class mat_skysphere : public material {
public:
	mat_skysphere(std::vector<vec4> colours) {
		num_cols = u16(colours.size());
		cols = new vec4[num_cols];
		for (u16 i = 0; i < num_cols; ++i)
			cols[i] = colours[i];
	}
	~mat_skysphere() {
		delete[]cols;
	}

	virtual bool scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const;

	vec4 * cols = nullptr;
	u16 num_cols = 0;
};

class mat_flat_colour : public material {
public:
	mat_flat_colour(const vec3& colour) : rgb(colour) {}
	virtual bool scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const;

	vec3 rgb;
};

class mat_tiles : public material {
public:
	mat_tiles(const vec3& colour_a, const vec3& colour_b, float rough) : a(colour_a), b(colour_b), roughness(rough) {}
	virtual bool scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const;

	vec3 a, b;
	float roughness;
};

class mat_normal : public material {
public:
	mat_normal() {}
	virtual bool scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const;
};

class mat_lambertian : public material {
public:
	mat_lambertian(const vec3& a, float rough) : albedo(a), roughness(rough) {}
	virtual bool scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const;

	vec3 albedo;
	float roughness;
};

class mat_diffuse : public material {
public:
	mat_diffuse(const vec3& a, float rough) : albedo(a), roughness(rough) {}
	virtual bool scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const;

	vec3 albedo;
	float roughness;
};

class mat_metal : public material {
public:
	mat_metal(const vec3& a, float rough) : albedo(a) { roughness = saturate(rough); }
	virtual bool scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const;
	vec3 albedo;
	float roughness;
};

const float RefIdxWater = 1.333f;
const float RefIdxEthanol = 1.36f;
const float RefIdxOliveOil = 1.47f;
const float RefIdxIce = 1.31f;
const float RefIdxQuartz = 1.46f;
const float RefIdxPerspex = 1.49f;
const float RefIdxGlass = 1.52f;
const float RefIdxPolycarbonate = 1.58f;
const float RefIdxFlintGlass = 1.62f;
const float RefIdxSapphire = 1.77f;
const float RefIdxCubicZirconia = 2.15f;
const float RefIdxDiamond = 2.42f;

class mat_dielectric : public material {
public:
	mat_dielectric(float ri) : ref_idx(ri) {
		if (ri < 1.0f)
			ri = 1.0f;
	}
	virtual bool scatter(const ray3& r, const hit_record& hit, vec3& col, ray3& scattered) const;

	float ref_idx;
};

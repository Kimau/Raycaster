#pragma once
#include "hit_record.h"
#include <vector>

enum material_type {
	MATERIAL_FLAT_COLOUR,
	MATERIAL_DIFFUSE,	// rgba : [roughness,?,?,?]
	MATERIAL_GLASS,
	MATERIAL_SKY,
	MATERIAL_NORMALS,
	MATERIAL_TILES,
	MATERIAL_LAMBERTIAN,
	NOOF_MATERIAL_TYPES
};

class material {
public:
	virtual material_type get_type() = 0;
	virtual bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) const = 0;
	virtual bool needsBounceBack() const { return false; }
	virtual void bounceBack(vec3& col) const {}
};

class mat_flat :public  material {
public:
	vec3 rgb;
	mat_flat(vec3 colour) : rgb(colour) {}

	inline material_type get_type() { return MATERIAL_FLAT_COLOUR; };
	bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) const override;
};

class mat_normal :public  material {
public:
	mat_normal() {}

	inline material_type get_type() { return MATERIAL_NORMALS; };
	bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) const override;
};

class mat_diffuse : public material {
public:
	float r = 0.0f;
	float m = 1.0f;

	mat_diffuse(float roughness, float mirror) : r(roughness), m(mirror) {}

	inline material_type get_type() { return MATERIAL_DIFFUSE; };
	bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) const override;

	void bounceBack(vec3& col) const override { col *= m; }
	inline bool needsBounceBack() const override { return m < 1.0f; }
};


class mat_lambertian : public material {
public:
	vec3 a; // albedo
	float r = 0.0f;
	float m = 1.0f;

	mat_lambertian(vec3 albedo, float roughness) : a(albedo), r(roughness), m(a.x*a.x + a.y*a.y + a.z*a.z) {}

	inline material_type get_type() { return MATERIAL_LAMBERTIAN; };
	bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) const override;

	void bounceBack(vec3& col) const override { col *= a; }
	inline bool needsBounceBack() const override { return m < 1.0f; }
};


class mat_glass : public material {
public:
	vec3 rgb;
	float r = 0.0f;

	mat_glass(float roughness, vec3 tint) : r(roughness), rgb(tint) {}

	inline material_type get_type() { return MATERIAL_GLASS; };
	bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) const override;

	void bounceBack(vec3& col) const override { col *= rgb; }
	inline bool needsBounceBack() const override { return rgb.SqrdMag() < 1.0f; }
};

class mat_tiles : public material {
public:
	float r = 0.0f;
	float m = 1.0f;

	mat_tiles(float roughness, float mirror) : r(roughness), m(mirror) {}

	inline material_type get_type() { return MATERIAL_DIFFUSE; };
	bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) const override;
};

class mat_sky : public material {
public:
	vec4 * cols = nullptr;
	u16 num_cols = 0;

	mat_sky(std::vector<vec4> colours) {
		num_cols = u16(colours.size());
		cols = new vec4[num_cols];
		for (u16 i = 0; i < num_cols; ++i)
			cols[i] = colours[i];
	}
	~mat_sky() {
		delete cols;
	}

	inline material_type get_type() { return MATERIAL_SKY; };
	bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) const override;
};
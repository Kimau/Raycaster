#pragma once
#include "hit_record.h"

enum solid_type
{
	SOLID_SKY,
	SOLID_BALL,
	SOLID_PLANE,
	SOLID_DISC,
	NOOF_SOLID_TYPES
};


class solidhit {
public:

	virtual bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const = 0;
	virtual u16 get_material() = 0;
	virtual void set_material(u16 mat) = 0;
	virtual solid_type get_type() = 0;
};

class solid_ball : public solidhit {
public:
	vec4 v;
	u16 mat = 0;

	solid_ball(vec4 vec) : v(vec), mat(0) {}

	inline u16 get_material() { return mat; }
	inline void set_material(u16 m) { mat = m; }
	inline solid_type get_type() { return SOLID_BALL; }

	bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const;
};

class solid_skysphere : public solidhit {
public:
	float rad = 0.0f;
	u16 mat = 0;

	solid_skysphere(float radius) : rad(radius) {}

	inline u16 get_material() { return mat; }
	inline void set_material(u16 m) { mat = m; }
	inline solid_type get_type() { return SOLID_SKY; }

	bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const override;
};

class  solid_plane : public solidhit {
public:
	vec4 v;
	u16 mat = 0;

	solid_plane(vec4 vec) : v(vec), mat(0) {}

	inline u16 get_material() { return mat; }
	inline void set_material(u16 m) { mat = m; }
	inline solid_type get_type() { return SOLID_PLANE; }

	bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const override;
};

class  solid_disc : public solidhit
{
public:
	vec4 v;
	float r2;
	u16 mat = 0;

	solid_disc(vec4 vec, float rad) : v(vec), r2(rad*rad), mat(0) {}

	inline u16 get_material() { return mat; }
	inline void set_material(u16 m) { mat = m; }
	inline solid_type get_type() { return SOLID_PLANE; }


	bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const override;

};

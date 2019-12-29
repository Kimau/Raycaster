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
	u16 mat = 0;

	virtual bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const = 0;
	virtual solid_type get_type() const = 0;
};

class solid_ball : public solidhit {
public:
	vec4 v;

	solid_ball(vec4 vec) : v(vec) {}
	inline solid_type get_type() const { return SOLID_BALL; }

	bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const;
};

class solid_skysphere : public solidhit {
public:
	float rad = 0.0f;

	solid_skysphere(float radius) : rad(radius) {}
	inline solid_type get_type() const { return SOLID_SKY; }

	bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const override;
};

class  solid_plane : public solidhit {
public:
	vec4 v;

	solid_plane(vec4 vec) : v(vec) {}
	inline solid_type get_type() const { return SOLID_PLANE; }

	bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const override;
};

class  solid_disc : public solidhit
{
public:
	vec4 v;
	float r2;

	solid_disc(vec4 vec, float rad) : v(vec), r2(rad*rad) {}
	inline solid_type get_type() const { return SOLID_PLANE; }


	bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const override;

};

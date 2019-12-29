#pragma once
#include "math/algebra.h"

class material;

class hit_record
{
public:
	vec3 p;
	vec3 n;
	const material* mat = nullptr;
	float t = 0.0f;
	u64 obj_idx = 0;
};


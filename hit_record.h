#pragma once
#include "math/algebra.h"

class hit_record
{
public:
	float t = 0.0f;
	vec3 p;
	vec3 n;
	u32 hitid = 0;
};


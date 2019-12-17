#pragma once

#include <cstdint>
#include "CVector3.h"

class Ray
{
public:
	Ray() : a(CVector3(0,0,0)), b(CVector3(0,0,1)) {}
	Ray(const Ray& r) : a(r.a), b(r.b) {}
    Ray(const CVector3& origin, const CVector3& dir) : a(origin), b(dir) {}

	inline CVector3 PointAt(float t) const { return a + b*t; }

	inline const CVector3& origin() const { return a; } 
    inline const CVector3& dir() const { return b; }

	CVector3 a,b;
};


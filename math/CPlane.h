#pragma once

#include "CVector3.h"

// Ax + By + Cz + D = 0
class plane {
public:
  //	Constructors
  plane();
  plane(float x, float y, float z, float d);
  plane(vec3 _normal);
  plane(vec3 _normal, vec3 _point);
  plane(vec3 _point1, vec3 _point2, vec3 _point3);
  //	Manipulators

  //	Accessors
  float PlaneDistance() const;           // Plane Distance from Origin
  float DistanceFrom(vec3 _point) const; // Distant between point and plane
  bool isIntersect(vec3 _start,
                   vec3 _end) const; // Returns if line intersects plane
  vec3 getIntersectPoint(vec3 _start,
                         vec3 _dir) const; // Get point of Intersection

  //	Variables
  vec3 m_normal;
  vec3 m_point;
};

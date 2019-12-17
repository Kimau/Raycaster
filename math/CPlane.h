#pragma once

#include "CVector3.h"

// Ax + By + Cz + D = 0
class CPlane {
public:
  //	Constructors
  CPlane();
  CPlane(float x, float y, float z, float d);
  CPlane(Vec3 _normal);
  CPlane(Vec3 _normal, Vec3 _point);
  CPlane(Vec3 _point1, Vec3 _point2, Vec3 _point3);
  //	Manipulators

  //	Accessors
  float PlaneDistance() const;           // Plane Distance from Origin
  float DistanceFrom(Vec3 _point) const; // Distant between point and plane
  bool isIntersect(Vec3 _start,
                   Vec3 _end) const; // Returns if line intersects plane
  Vec3 getIntersectPoint(Vec3 _start,
                         Vec3 _dir) const; // Get point of Intersection

  //	Variables
  Vec3 m_normal;
  Vec3 m_point;
};

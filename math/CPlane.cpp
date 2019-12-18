#include "CPlane.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	CONSTRUCTORS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Create "Blank" plane
plane::plane() : m_normal(vec3(0, 0, 1)), m_point(vec3(0, 0, 0)) {}
//	Create Plane from 4 floats : Ax + By + Cz + D = 0
plane::plane(float x, float y, float z, float d) : m_normal(vec3(x, y, z)) {
  m_normal.Normalize();
  m_point = m_normal * -d;
}
//	Create Plane from Normal and orig
plane::plane(vec3 _normal) : m_normal(_normal), m_point(vec3(0, 0, 0)) {}
//	Create Plane from Normal and point on Plane
plane::plane(vec3 _normal, vec3 _point)
    : m_normal(_normal), m_point(_point) {}
//	Create Plane from 3 Points
plane::plane(vec3 _point1, vec3 _point2, vec3 _point3) : m_point(_point1) {
  m_normal = (_point1 - _point2) * (_point2 - _point3);
  m_normal.Normalize();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	ACCESSORS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	Get Plane Distance from Orig
float plane::PlaneDistance() const { return (-m_normal.Dot(m_point)); }
//	Distant between point and plane
float plane::DistanceFrom(vec3 _point) const {
  return (-m_normal.Dot(_point) + PlaneDistance());
}
// Returns if line intersects plane
bool plane::isIntersect(vec3 _start, vec3 _end) const {
  return ((DistanceFrom(_start) > 0) != (DistanceFrom(_end) > 0));
}
// Get point of Intersection
vec3 plane::getIntersectPoint(vec3 _start, vec3 _dir) const {
  _dir.Normalize();

  // Place both in same frame
  float _numer = m_point.Magnitude() + m_normal.x + _start.x + m_normal.y +
                 _start.y + m_normal.z + _start.z;

  //	Get Angle
  float _denom = m_normal.Dot(_dir);

  //	Line is axis of plane
  if (_denom == 0.0f)
    return m_point;

  //	Get Distance to intersect
  float _dist = _numer / _denom;

  return (_start + (_dir * _dist));
}

#include "CVector2.h"
#include <float.h>
#include <math.h>

//		Functions
//	Normalizes the vector
void vec2::Normalize() {
  // Get the magnitude
  float _Magnitude = Magnitude();

  // Recalculate Lengths
  x /= _Magnitude;
  y /= _Magnitude;
}
//	Inverts all the values of the Vector
void vec2::Invert() {
  x = -x;
  y = -y;
}

//		Accessors
//	Returns Normalized form of the Vector
vec2 vec2::getNormalized() const {
  // Get the magnitude
  float _Magnitude = Magnitude();
  vec2 _normalized(x, y);

  // Recalculate Lengths
  _normalized.x /= _Magnitude;
  _normalized.y /= _Magnitude;

  return _normalized;
}
//	Returns the maginitude of the vector
float vec2::Magnitude() const { return (sqrtf(x * x + y * y)); }
//	Gets the Dot product of the Vectors

//	Gets the angle between
float vec2::Angle(vec2 _vector) const {
  float _angle = acosf((dot(*this, _vector) / (Magnitude() * _vector.Magnitude())));

  if (isnan(_angle))
    return 0;

  // Return the angle in radians
  return (_angle);
}

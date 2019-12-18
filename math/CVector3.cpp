#include "CVector3.h"
#include <float.h>
#include <math.h>

//		Functions
//	Rotate Around the X axis
void vec3::RotateX(float _degree) {
  float _rad = _degree * DEG_TO_RAD;
  float _cosAng = cosf(-_rad);
  float _sinAng = sinf(-_rad);

  float _nY = y * _cosAng - z * _sinAng;
  float _nZ = z * _cosAng + y * _sinAng;

  y = _nY;
  z = _nZ;
}
//	Rotate Around the Y axis
void vec3::RotateY(float _degree) {
  float _rad = _degree * DEG_TO_RAD;
  float _cosAng = cosf(-_rad);
  float _sinAng = sinf(-_rad);

  float _nX = x * _cosAng + z * _sinAng;
  float _nZ = z * _cosAng - x * _sinAng;

  x = _nX;
  z = _nZ;
}
//	Rotate Around the Z axis
void vec3::RotateZ(float _degree) {
  float _rad = _degree * DEG_TO_RAD;
  float _cosAng = cosf(-_rad);
  float _sinAng = sinf(-_rad);

  float _nX = x * _cosAng - y * _sinAng;
  float _nY = y * _cosAng + x * _sinAng;

  x = _nX;
  y = _nY;
}
//	Rotate Around the given axis
void vec3::Rotate(float _degree, vec3 _axis) {
  //	Get Vars
  float _rad = _degree * DEG_TO_RAD;
  float _cosAng = cosf(_rad);
  float _sinAng = sinf(_rad);

  _axis.Normalize();
  vec3 _Cross = vec3(x, y, z) * _axis;
  _axis = _axis * Dot(_axis);

  //	Rotate
  x = _axis.x + (x - _axis.x) * _cosAng + _Cross.x * _sinAng;
  y = _axis.y + (y - _axis.y) * _cosAng + _Cross.y * _sinAng;
  z = _axis.z + (z - _axis.z) * _cosAng + _Cross.z * _sinAng;
}
//	Normalizes the vector
void vec3::Normalize() {
  // Get the magnitude
  float _Magnitude = Magnitude();
  if ((_Magnitude < 0.000000001f) || (isnan(_Magnitude))) {
    x = 0;
    y = 0;
    z = 0;
  } else {
    x /= _Magnitude;
    y /= _Magnitude;
    z /= _Magnitude;
  }
}
//	Inverts all the values of the Vector
void vec3::Invert() {
  x = -x;
  y = -y;
  z = -z;
}

//		Accessors
//	Returns Normalized form of the Vector
vec3 vec3::getNormalized() const {
  // Get the magnitude
  float _Magnitude = Magnitude();
  if ((_Magnitude < 0.000000001f) || (isnan(_Magnitude))) {
    return vec3(0, 0, 0);
  } else {
    vec3 _normalized(x, y, z);

    // Recalculate Lengths
    _normalized.x /= _Magnitude;
    _normalized.y /= _Magnitude;
    _normalized.z /= _Magnitude;
    return _normalized;
  }
}
//	Returns the Longitude of given point assuming (this) is a point
//	on a sphere. Longitude is given in form of a UNIT vector going
//	NORTH - SOUTH : North is positive
vec3 vec3::getLongitude() const {
  vec3 _long(-(z * x), -(z * y), (x * x) + (y * y));
  _long.Normalize();
  return _long;
}
//	Returns the Latitude of given point assuming (this) is a point
//	on a sphere. Latitude is given in form of a UNIT vector going
//	EAST - WEST : East is positive
vec3 vec3::getLatitude() const {
  vec3 _lat(-y, x, 0.0f);
  _lat.Normalize();
  return _lat;
}
//	Returns the maginitude of the vector
float vec3::Magnitude() const { return (sqrtf(x * x + y * y + z * z)); }
//	Returns the Squared Magnitude of a Vector
float vec3::SqrdMag() const { return x * x + y * y + z * z; }
//	Gets the Dot product of the Vectors
float vec3::Dot(vec3 _vector) const {
  return ((x * _vector.x) + (y * _vector.y) + (z * _vector.z));
}
//	Gets the angle between
float vec3::Angle(vec3 _vector) const {
  // Get the arc cosine of the (dotProduct / vectorsMagnitude) which is the
  // angle in RADIANS.
  // (IE.   PI/2 radians = 90 degrees      PI radians = 180 degrees    2*PI
  // radians = 360 degrees)
  // To convert radians to degress use this equation:   radians * (PI / 180)
  // TO convert degrees to radians use this equation:   degrees * (180 / PI)
  float _angle = acosf(Dot(_vector) / (Magnitude() * _vector.Magnitude()));

  // Here we make sure that the angle is not a -1.#IND0000000 number, which
  // means indefinite.
  // acos() thinks it's funny when it returns -1.#IND0000000.  If we don't do
  // this check,
  // our collision results will sometimes say we are colliding when we aren't.
  // I found this
  // out the hard way after MANY hours and already wrong written tutorials :)
  // Usually
  // this value is found when the dot product and the magnitude are the same
  // value.
  // We want to return 0 when this happens.
  if (isnan(_angle))
    return 0;

  // Return the angle in radians
  return (_angle);
}
//	Gets the distance to the closest point on the line
float vec3::Distance(vec3 _point, vec3 _orig) const {
  _point -= _orig;
  vec3 _top = (_point * (*this));
  _top /= Magnitude();
  return _top.Magnitude();
}
//	Gets the distance to the closest point on the line
//	ASSUMES (this) is from Origin and length 1
float vec3::DistanceFast(vec3 _point) const {
  vec3 _top = (_point * (*this));
  return _top.Magnitude();
}

//	Returns an array for use
//	ASSUMES ARRAY IS OF LENGTH 3 OR GREATER
void vec3::getArray(float *_array) const {
  _array[0] = x;
  _array[1] = y;
  _array[2] = z;
}
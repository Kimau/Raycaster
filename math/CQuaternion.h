#pragma once

//	Defines
#define PI 3.141592654f // Simpler PI
#define GET_RADIANS(deg)                                                       \
  (float)((deg * PI) / 180.0f) // Quick Convert Deg to Rad

//	Includes
#include "CVector3.h"
#include <math.h>

//	Quaternion Class
//	Float based so beware floating Errors
class Quat {
public:
  //	Constructors
  Quat();
  Quat(float _x, float _y, float _z, float _w);
  Quat(Vec3 _axis, float _rad);

  //	Operators
  Quat operator*(const Quat &_other);

  //	Self Operators
  void operator=(const Quat &_other);
  void operator*=(const Quat &_other);

  //	Functions
  void Conjugate();
  void Normalize();
  void CreateMatrix(float *_matrix);
  //	Accessors
  Quat getNormalized();
  Quat getConjugated();

  float x, y, z, w;
};

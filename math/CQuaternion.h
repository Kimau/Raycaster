#pragma once
#include "../types.h"

//	Quaternion Class
//	Float based so beware floating Errors
struct quat {
public:
  //	Constructors
  quat();
  quat(float _x, float _y, float _z, float _w);
  quat(vec3 _axis, float _rad);

  //	Operators
  quat operator*(const quat &_other);

  //	Self Operators
  void operator=(const quat &_other);
  void operator*=(const quat &_other);

  //	Functions
  void Conjugate();
  void Normalize();
  void CreateMatrix(float *_matrix);
  //	Accessors
  quat getNormalized();
  quat getConjugated();

  float x, y, z, w;
};

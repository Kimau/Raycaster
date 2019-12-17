#include "CQuaternion.h"

//############################
//############################
//		CONSTRUCTORS
//############################
//############################
//==========================================================================
//	Blank Constructor
//==========================================================================
Quat::Quat() : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{}
//==========================================================================
//	Initiated Constructor
//==========================================================================
Quat::Quat(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
{}
//==========================================================================
//	Quat from Axis
//==========================================================================
Quat::Quat(Vec3 _axis, float _rad)
{
	_axis.Normalize();

	float _sine = sinf(_rad *0.5f);

	x = _axis.x * _sine;
	y = _axis.y * _sine;
	z = _axis.z * _sine;
	w = cosf(_rad *0.5f);

	Normalize();
}

//############################
//############################
//		OPERATORS
//############################
//############################
//==========================================================================
//	Multiplication 
//	Q1 (dot) Q2 + Q1 (cross) Q2
//==========================================================================
Quat Quat::operator*(const Quat& _other)
{
    Quat _retVaule;

    _retVaule.x = w * _other.x + x * _other.w + y * _other.z - z * _other.y;
    _retVaule.y = w * _other.y - x * _other.z + y * _other.w + z * _other.x;
    _retVaule.z = w * _other.z + x * _other.y - y * _other.x + z * _other.w;
    _retVaule.w = w * _other.w - x * _other.x - y * _other.y - z * _other.z;

    return _retVaule;
}
//############################
//############################
//		SELF OPERATORS
//############################
//############################
//==========================================================================
//	Assignment Operator
//==========================================================================
void Quat::operator=(const Quat& _other)
{
	x = _other.x;
	y = _other.y;
	z = _other.z;
	w = _other.w;
}
//==========================================================================
//	Multiplication with Self
//==========================================================================
void Quat::operator*=(const Quat& _other)
{
    Quat _retVaule;

    _retVaule.x = w * _other.x + x * _other.w + y * _other.z - z * _other.y;
    _retVaule.y = w * _other.y - x * _other.z + y * _other.w + z * _other.x;
    _retVaule.z = w * _other.z + x * _other.y - y * _other.x + z * _other.w;
    _retVaule.w = w * _other.w - x * _other.x - y * _other.y - z * _other.z;

    x = _retVaule.x;
	y = _retVaule.y;
	z = _retVaule.z;
	w = _retVaule.w;
}

//############################
//############################
//		FUNCTIONS
//############################
//############################
//==========================================================================
//	Negate everything except w
//==========================================================================
void Quat::Conjugate()
{ 
	x = -x;
	y = -y;
	z = -z;
}
//==========================================================================
//	Normalize Q
//==========================================================================
void Quat::Normalize()
{
	float _length = 1.0f / sqrtf(x * x + y * y + z * z + w * w);
    x *= _length;
	y *= _length;
	z *= _length;
}

//==========================================================================
//	Creates a 4x4 Transform Matrix
//==========================================================================
void Quat::CreateMatrix(float* _matrix = 0)
{
	//------------------------
	//	Allocate Memory
	//------------------------
	if(!_matrix)
		_matrix = new float[16];

	//------------------------
	//	Calculate
	//------------------------
	_matrix[0]  = 1.0f - 2.0f * (y * y + z * z); 
	_matrix[1]  =		 2.0f * (x * y + z * w);
	_matrix[2]  =		 2.0f * (x * z - y * w);
	_matrix[3]  = 0.0f;  

	_matrix[4]  =		 2.0f * (x * y - z * w);  
	_matrix[5]  = 1.0f - 2.0f * (x * x + z * z); 
	_matrix[6]  =	     2.0f * (z * y + x * w);  
	_matrix[7]  = 0.0f;  

	_matrix[8]  =		 2.0f * (x * z + y * w);
	_matrix[9]  =		 2.0f * (y * z - x * w);
	_matrix[10] = 1.0f - 2.0f * (x * x + y * y);  
	_matrix[11] = 0.0f;  

	_matrix[12] = 0;  
	_matrix[13] = 0;  
	_matrix[14] = 0;  
	_matrix[15] = 1.0f;
}
//############################
//############################
//		ACCESORS
//############################
//############################
//==========================================================================
//	Get Normalize version of Q
//==========================================================================
Quat Quat::getNormalized()
{
	float _length = 1.0f / sqrtf(x * x + y * y + z * z + w * w);
	return Quat(x * _length,y * _length,z * _length, w);
}
//==========================================================================
//	Get Conjugated version of Q
//==========================================================================
Quat Quat::getConjugated()
{
	return Quat(-x,-y,-z, w);
}
#include "CVector2.h"
#include <math.h>
#include <float.h>


//####################################################################################################
//####################################################################################################
//		Constructors
//####################################################################################################
//####################################################################################################
//	Default Constructor
Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(float _X, float _Y) : x(_X), y(_Y) {}
Vec2::Vec2(long  _X, long  _Y) : x(float(_X)), y(float(_Y)) {}
Vec2::Vec2(int _X, int _Y) : x(float(_X)), y(float(_Y)) {}

//	Copy Constructor
Vec2::Vec2(const Vec2& _vector)
{
	if(this != &_vector)
	{
		x = _vector.x;
		y = _vector.y;
	}
}

//####################################################################################################
//####################################################################################################
//		Operators
//####################################################################################################
//####################################################################################################
//	Addition Operator
Vec2 Vec2::operator+(Vec2 _vector)
{
	return Vec2((x+_vector.x),(y+_vector.y));
}
//	Subtraction Operator
Vec2 Vec2::operator-(Vec2 _vector)
{
	return Vec2((x-_vector.x),(y-_vector.y));
}

//	Scalar Multiplication
Vec2 Vec2::operator*(float _num)
{
	return Vec2((x*_num),(y*_num));
}
//	Scalar Division
Vec2 Vec2::operator/(float _num)
{
	return Vec2((x/_num),(y/_num));
}

//	Returns the Magnitude of two vectors divided
float Vec2::operator/(Vec2 _vector)
{
	return ((Magnitude()) / (_vector.Magnitude()));
}
//	Check Magnitude
//	Due to the nature of floats it checks within a 5% tolerance
bool Vec2::operator==(float _mag) const
{
	float _result = Magnitude() / _mag;	
	return ((_result < 1.05f) || (_result > 0.95f));
}
//	Equal Comparison for Vectors
//	Due to the nature of floats it checks within a 5% tolerance
bool Vec2::operator==(Vec2 _vector) const
{
	float _result = ((x/_vector.x) + (y/_vector.y)) / 2.0f;
	return ((_result < 1.05f) || (_result > 0.95f));
}
//	Checks for inequality
//	Uses equality operator
bool Vec2::operator!=(Vec2 _vector) const
{
	return !((*this) == _vector);
}

//	Addition Operator
void Vec2::operator+=(Vec2 _vector)
{
	x += _vector.x;
	y += _vector.y;
}
//	Substraction Operator
void Vec2::operator-=(Vec2 _vector)
{
	x -= _vector.x;
	y -= _vector.y;
}

//	Scalar Multiplication
void Vec2::operator*=(float _num)
{
	x *= _num;
	y *= _num;
}
//	Scalar Division
void Vec2::operator/=(float _num)
{
	x /= _num;
	y /= _num;
}

//	Returns the Maginitude of two vectors divided
void Vec2::operator/=(Vec2 _vector)
{
	float _mag = _vector.Magnitude();
	x /= _mag;
	y /= _mag;
}

//####################################################################################################
//####################################################################################################
//		Functions
//####################################################################################################
//####################################################################################################
//	Normalizes the vector
void Vec2::Normalize()
{
	// Get the magnitude
	float _Magnitude = Magnitude();

	// Recalculate Lengths
	x /= _Magnitude;
	y /= _Magnitude;
}
//	Inverts all the values of the Vector
void Vec2::Invert()
{
	x = -x;
	y = -y;
}

//####################################################################################################
//####################################################################################################
//		Accessors
//####################################################################################################
//####################################################################################################
//	Returns Normalized form of the Vector
Vec2 Vec2::getNormalized() const
{
	// Get the magnitude
	float _Magnitude = Magnitude();
	Vec2 _normalized(x,y);

	// Recalculate Lengths
	_normalized.x /= _Magnitude;
	_normalized.y /= _Magnitude;

	return _normalized;
}
//	Returns the maginitude of the vector
float Vec2::Magnitude() const
{
	return (sqrtf(x*x + y*y));
}
//	Gets the Dot product of the Vectors
float Vec2::Dot(Vec2 _vector) const
{
	return ((x * _vector.x) + (y * _vector.y));
}
//	Gets the angle between
float Vec2::Angle(Vec2 _vector) const
{
	float _angle = acosf((Dot(_vector) / (Magnitude() * _vector.Magnitude())));

	if(isnan(_angle))
		return 0;

	// Return the angle in radians
	return(_angle);
}

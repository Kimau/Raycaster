#include "CVector3.h"
#include <math.h>
#include <float.h>


//####################################################################################################
//####################################################################################################
//		Constructors
//####################################################################################################
//####################################################################################################
//--------------------------------------------------------------------
//	Default Constructor
//--------------------------------------------------------------------
Vec3::Vec3() : x(0), y(0), z(0)
{}
//--------------------------------------------------------------------
//	Initated Constructor
//--------------------------------------------------------------------
Vec3::Vec3(float _X, float _Y, float _Z) : x(_X), y(_Y), z(_Z)
{}
//--------------------------------------------------------------------
//	Initated Constructor
//--------------------------------------------------------------------
Vec3::Vec3(double _X, double _Y, double _Z) : x((float)_X), y((float)_Y), z((float)_Z)
{}
//--------------------------------------------------------------------
//	Initated Constructor
//--------------------------------------------------------------------
Vec3::Vec3(uint16_t _X, uint16_t _Y, uint16_t _Z) : x((float)_X), y((float)_Y), z((float)_Z)
{}
//--------------------------------------------------------------------
//	Initated Constructor
//--------------------------------------------------------------------
Vec3::Vec3(uint16_t _X, uint16_t _Y, float _Z) : x((float)_X), y((float)_Y), z(_Z)
{}
//--------------------------------------------------------------------
//	Initated Constructor
//--------------------------------------------------------------------
Vec3::Vec3(int _X, int _Y, int _Z) : x((float)_X), y((float)_Y), z((float)_Z)
{}
//--------------------------------------------------------------------
//	Copy Constructor
//--------------------------------------------------------------------
Vec3::Vec3(const Vec3& _vector)
{
	if(this != &_vector)
	{
		x = _vector.x;
		y = _vector.y;
		z = _vector.z;
	}
}

//####################################################################################################
//####################################################################################################
//		Operators
//####################################################################################################
//####################################################################################################
//--------------------------------------------------------------------
//	Addition Operator
//--------------------------------------------------------------------
Vec3 Vec3::operator+(Vec3 _vector)
{
	return Vec3(
		(x+_vector.x),
		(y+_vector.y),
		(z+_vector.z));
}
//--------------------------------------------------------------------
//	Substraction Operator
//--------------------------------------------------------------------
Vec3 Vec3::operator-(Vec3 _vector)
{
	return Vec3(
		(x-_vector.x),
		(y-_vector.y),
		(z-_vector.z));
}
//--------------------------------------------------------------------
//	Cross Product of Two Vectors
//--------------------------------------------------------------------
Vec3 Vec3::operator*(Vec3 _vector)
{
	Vec3 _vCross;									// The vector to hold the cross product
	_vCross.x = ((y * _vector.z) - (z * _vector.y));	// Get the X value
	_vCross.y = ((z * _vector.x) - (x * _vector.z));	// Get the Y value												
	_vCross.z = ((x * _vector.y) - (y * _vector.x));	// Get the Z value

	return _vCross;										// Return the cross product
}

//--------------------------------------------------------------------
//	Scalar Multiplication
//--------------------------------------------------------------------
Vec3 Vec3::operator*(float _num)
{
	return Vec3(
		(x*_num),
		(y*_num),
		(z*_num));
}
//--------------------------------------------------------------------
//	Scalar Division
//--------------------------------------------------------------------
Vec3 Vec3::operator/(float _num)
{
	return Vec3(
		(x/_num),
		(y/_num),
		(z/_num));
}

//--------------------------------------------------------------------
//	Returns the Maginitude of two vectors divided
//--------------------------------------------------------------------
float Vec3::operator/(Vec3 _vector)
{
	return ((Magnitude()) / (_vector.Magnitude()));
}
//--------------------------------------------------------------------
//	Check Magnitude
//	Due to the nature of floats it checks within a 5% tolerance
//--------------------------------------------------------------------
bool Vec3::operator==(float _mag) const
{
	float _result = Magnitude() / _mag;	
	return ((_result < 1.05f) || (_result > 0.95f));
}
//--------------------------------------------------------------------
//	Equal Comparison for Vectors
//	Due to the nature of floats it checks within a 5% tolerance
//--------------------------------------------------------------------
bool Vec3::operator==(Vec3 _vector) const
{
	float _result = ((x/_vector.x) + (y/_vector.y) + (z/_vector.z)) / 3.0f;
	return ((_result < 1.05f) || (_result > 0.95f));
}
//--------------------------------------------------------------------
//	Checks for inquality
//	Uses equality operator
//--------------------------------------------------------------------
bool Vec3::operator!=(Vec3 _vector) const
{
	return !((*this) == _vector);
}
//--------------------------------------------------------------------
//	Addition Operator
//--------------------------------------------------------------------
void Vec3::operator+=(Vec3 _vector)
{
	x += _vector.x;
	y += _vector.y;
	z += _vector.z;
}
//--------------------------------------------------------------------
//	Substraction Operator
//--------------------------------------------------------------------
void Vec3::operator-=(Vec3 _vector)
{
	x -= _vector.x;
	y -= _vector.y;
	z -= _vector.z;
}
//--------------------------------------------------------------------
//	Cross Product of Two Vectors
//--------------------------------------------------------------------
void Vec3::operator*=(Vec3 _vector)
{
	x = ((y * _vector.z) - (z * _vector.y));	// Get the X value
	y = ((z * _vector.x) - (x * _vector.z));	// Get the Y value												
	z = ((x * _vector.y) - (y * _vector.x));	// Get the Z value
}

//--------------------------------------------------------------------
//	Scalar Multiplication
//--------------------------------------------------------------------
void Vec3::operator*=(float _num)
{
	x *= _num;
	y *= _num;
	z *= _num;
}
//--------------------------------------------------------------------
//	Scalar Division
//--------------------------------------------------------------------
void Vec3::operator/=(float _num)
{
	x /= _num;
	y /= _num;
	z /= _num;
}

//--------------------------------------------------------------------
//	Returns the Maginitude of two vectors divided
//--------------------------------------------------------------------
void Vec3::operator/=(Vec3 _vector)
{
	float _mag = _vector.Magnitude();
	x /= _mag;
	y /= _mag;
	z /= _mag;
}

//####################################################################################################
//####################################################################################################
//		Functions
//####################################################################################################
//####################################################################################################
//--------------------------------------------------------------------
//	Rotate Around the X axis
//--------------------------------------------------------------------
void Vec3::RotateX(float _degree)
{
	float _rad = _degree * DEG_TO_RAD;
	float _cosAng = cosf(-_rad);
	float _sinAng = sinf(-_rad);

	float _nY = y * _cosAng - z * _sinAng;
	float _nZ = z * _cosAng + y * _sinAng;

	y = _nY;
	z = _nZ;
}
//--------------------------------------------------------------------
//	Rotate Around the Y axis
//--------------------------------------------------------------------
void Vec3::RotateY(float _degree)
{
	float _rad = _degree * DEG_TO_RAD;
	float _cosAng = cosf(-_rad);
	float _sinAng = sinf(-_rad);
	
	float _nX = x * _cosAng + z * _sinAng;
	float _nZ = z * _cosAng - x * _sinAng;

	x = _nX;
	z = _nZ;
}
//--------------------------------------------------------------------
//	Rotate Around the Z axis
//--------------------------------------------------------------------
void Vec3::RotateZ(float _degree)
{
	float _rad = _degree * DEG_TO_RAD;
	float _cosAng = cosf(-_rad);
	float _sinAng = sinf(-_rad);

	float _nX = x * _cosAng - y * _sinAng;
	float _nY = y * _cosAng + x * _sinAng;
	
	x = _nX;
	y = _nY;
}
//--------------------------------------------------------------------
//	Rotate Around the given axis
//--------------------------------------------------------------------
void Vec3::Rotate(float _degree, Vec3 _axis)
{
	//------------------------------------
	//	Get Vars
	//------------------------------------
	float _rad = _degree * DEG_TO_RAD;
	float _cosAng = cosf(_rad);
	float _sinAng = sinf(_rad);
	
	_axis.Normalize();
	Vec3 _Cross = Vec3(x,y,z) * _axis;
	_axis = _axis * Dot(_axis);

	//------------------------------------
	//	Rotate
	//------------------------------------
	x = _axis.x + (x - _axis.x) * _cosAng + _Cross.x * _sinAng;
	y = _axis.y + (y - _axis.y) * _cosAng + _Cross.y * _sinAng;
	z = _axis.z + (z - _axis.z) * _cosAng + _Cross.z * _sinAng;
}
//--------------------------------------------------------------------
//	Normalizes the vector
//--------------------------------------------------------------------
void Vec3::Normalize()
{
	// Get the magnitude
	float _Magnitude = Magnitude();
	if((_Magnitude < 0.000000001f) || (isnan(_Magnitude)))
	{
		x = 0;
		y = 0;
		z = 0;
	}
	else
	{
		x /= _Magnitude;
		y /= _Magnitude;
		z /= _Magnitude;
	}
}
//--------------------------------------------------------------------
//	Inverts all the values of the Vector
//--------------------------------------------------------------------
void Vec3::Invert()
{
	x = -x;
	y = -y;
	z = -z;
}

//####################################################################################################
//####################################################################################################
//		Accessors
//####################################################################################################
//####################################################################################################
//--------------------------------------------------------------------
//	Returns Normalized form of the Vector
//--------------------------------------------------------------------
Vec3 Vec3::getNormalized() const
{
	// Get the magnitude
	float _Magnitude = Magnitude();
	if((_Magnitude < 0.000000001f) || (isnan(_Magnitude)))
	{
		return Vec3(0,0,0);
	}
	else
	{
		Vec3 _normalized(x,y,z);

		// Recalculate Lengths
		_normalized.x /= _Magnitude;
		_normalized.y /= _Magnitude;
		_normalized.z /= _Magnitude;
		return _normalized;
	}
}
//--------------------------------------------------------------------
//	Returns the Longitude of given point assuming (this) is a point
//	on a sphere. Longitude is given in form of a UNIT vector going
//	NORTH - SOUTH : North is positive
//--------------------------------------------------------------------
Vec3 Vec3::getLongitude() const
{
	Vec3 _long(-(z*x),-(z*y),(x*x) + (y*y));
	_long.Normalize();
	return _long;
}
//--------------------------------------------------------------------
//	Returns the Latitude of given point assuming (this) is a point
//	on a sphere. Latitude is given in form of a UNIT vector going
//	EAST - WEST : East is positive
//--------------------------------------------------------------------
Vec3 Vec3::getLatitude() const
{
	Vec3 _lat(-y,x,0.0f);
	_lat.Normalize();
	return _lat;
}
//--------------------------------------------------------------------
//	Returns the maginitude of the vector
//--------------------------------------------------------------------
float Vec3::Magnitude() const
{
	return (sqrtf(x*x + y*y + z*z));
}
//--------------------------------------------------------------------
//	Returns the Squared Magnitude of a Vector
//--------------------------------------------------------------------
float Vec3::SqrdMag() const
{
	return x*x + y*y + z*z;
}
//--------------------------------------------------------------------
//	Gets the Dot product of the Vectors
//--------------------------------------------------------------------
float Vec3::Dot(Vec3 _vector) const
{
	return ((x * _vector.x) + (y * _vector.y) + (z * _vector.z));
}
//--------------------------------------------------------------------
//	Gets the angle between
//--------------------------------------------------------------------
float Vec3::Angle(Vec3 _vector) const
{
	// Get the arc cosine of the (dotProduct / vectorsMagnitude) which is the angle in RADIANS.
	// (IE.   PI/2 radians = 90 degrees      PI radians = 180 degrees    2*PI radians = 360 degrees)
	// To convert radians to degress use this equation:   radians * (PI / 180)
	// TO convert degrees to radians use this equation:   degrees * (180 / PI)
	float _angle = acosf(Dot(_vector) / (Magnitude() * _vector.Magnitude()));

	// Here we make sure that the angle is not a -1.#IND0000000 number, which means indefinite.
	// acos() thinks it's funny when it returns -1.#IND0000000.  If we don't do this check,
	// our collision results will sometimes say we are colliding when we aren't.  I found this
	// out the hard way after MANY hours and already wrong written tutorials :)  Usually
	// this value is found when the dot product and the magnitude are the same value.
	// We want to return 0 when this happens.
	if(isnan(_angle))
		return 0;

	// Return the angle in radians
	return(_angle);
}
//--------------------------------------------------------------------
//	Gets the distance to the closest point on the line
//--------------------------------------------------------------------
float Vec3::Distance(Vec3 _point, Vec3 _orig) const
{
	_point -= _orig;
	Vec3 _top = (_point * (*this));
	_top /= Magnitude();
	return _top.Magnitude();
}
//--------------------------------------------------------------------
//	Gets the distance to the closest point on the line
//	ASSUMES (this) is from Origin and length 1
//--------------------------------------------------------------------
float Vec3::DistanceFast(Vec3 _point) const
{
	Vec3 _top = (_point * (*this));
	return _top.Magnitude();
}

//--------------------------------------------------------------------
//	Returns an array for use
//	ASSUMES ARRAY IS OF LENGTH 3 OR GREATER
//--------------------------------------------------------------------
void Vec3::getArray(float* _array) const
{
	_array[0] = x;
	_array[1] = y;
	_array[2] = z;
}
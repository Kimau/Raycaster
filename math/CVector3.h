#pragma once

#include <cstdint>

#define DEG_TO_RAD 0.017453292519943f

//#########################################################################
//	3D Vector Class
//	Float base, note floating point errors
//#########################################################################
class Vec3
{
public:
	//-------------------------------------------
	//	Constructors
	//-------------------------------------------
	Vec3();
	Vec3(float _X, float _Y, float _Z);
	Vec3(double _X, double _Y, double _Z);
	Vec3(uint16_t _X, uint16_t _Y, uint16_t _Z);
	Vec3(uint16_t _X, uint16_t _Y, float _Z);
	Vec3(int _X, int _Y, int _Z);
	Vec3(const Vec3& _vector);

	//-------------------------------------------
	//	Operators
	//-------------------------------------------
	Vec3 operator+(Vec3 _vector);
	Vec3 operator-(Vec3 _vector);	
	Vec3 operator*(Vec3 _vector);
	Vec3 operator*(float _num);
	Vec3 operator/(float _num);
	float operator/(Vec3 _vector);

	//-------------------------------------------
	//	Self Operators
	//-------------------------------------------
	void operator+=(Vec3 _vector);
	void operator-=(Vec3 _vector);	
	void operator*=(Vec3 _vector);
	void operator*=(float _num);
	void operator/=(float _num);
	void operator/=(Vec3 _vector);

	//-------------------------------------------
	//	Comparison Operators
	//-------------------------------------------
	bool operator==(float _mag) const;
	bool operator==(Vec3 _vector) const;
	bool operator!=(Vec3 _vector) const;

	//-------------------------------------------
	//	Functions
	//-------------------------------------------
	void RotateX(float _degree);
	void RotateY(float _degree);
	void RotateZ(float _degree);
	void Rotate(float _degree, Vec3 _axis);
	void Normalize();
	void Invert();

	//-------------------------------------------
	//	Accesors
	//-------------------------------------------
	Vec3 getNormalized() const;
	Vec3 getLongitude() const;
	Vec3 getLatitude() const;
	float Magnitude() const;
	float SqrdMag() const;
	float Dot(Vec3 _vector) const;
	float Angle(Vec3 _vector) const;
	float Distance(Vec3 _point, Vec3 _orig = Vec3(0,0,0)) const;
	float DistanceFast(Vec3 _point) const;
	void  getArray(float* _array) const;

	float x,y,z;
};

typedef Vec3 CVert;
typedef Vec3 CNorm;

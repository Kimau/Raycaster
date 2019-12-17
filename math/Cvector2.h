#pragma once

//#########################################################################
//	2D Vector Class
//	Float base, note floating point errors
//#########################################################################
class Vec2
{
public:
	Vec2();
	Vec2(int _X, int _Y);
	Vec2(float _X, float _Y);
	Vec2(long  _X, long  _Y);
	Vec2(const Vec2& _vector);

		//	Operators
		Vec2 operator+(Vec2 _vector);
	Vec2 operator-(Vec2 _vector);	
	Vec2 operator*(float _num);
	Vec2 operator/(float _num);
	float operator/(Vec2 _vector);

		//	Self Operators
		void operator+=(Vec2 _vector);
	void operator-=(Vec2 _vector);	
	void operator*=(float _num);
	void operator/=(float _num);
	void operator/=(Vec2 _vector);

		//	Comparison Operators
		bool operator==(float _mag) const;
	bool operator==(Vec2 _vector) const;
	bool operator!=(Vec2 _vector) const;

	void Normalize();
	void Invert();
	Vec2 getNormalized() const;
	float Magnitude() const;
	float Dot(Vec2 _vector) const;
	float Angle(Vec2 _vector) const;

	float x,y;
};

typedef Vec2 CTexCoord;

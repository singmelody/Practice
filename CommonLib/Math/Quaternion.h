#pragma once
#include <math.h>
namespace Dream
{

class Quaternion
{
public:
	Quaternion( float _x, float _y, float _z, float _w)
	{
		x = _x; y = _y; z = _z; w = _w;
	};

	~Quaternion(){}

	float x,y,z,w;

	void Normalize()
	{
		float len = sqrt(x * x + y * y + z * z + w * w);
		float factor = 1.0f / len;
		x *= factor;
		y *= factor;
		z *= factor;
		w *= factor;
	}

	Quaternion Multiply(const Quaternion& q)
	{
		x = w * q.x + x * q.w + y * q.z - z * q.y;
		y = w * q.y + y * q.w + z * q.x - x * q.z;
		z = w * q.z + z * q.w + x * q.y - y * q.x;
		w = w * q.w - x * q.x - y * q.y - z * q.z;
	}

	Quaternion Quaternion::operator+ (const Quaternion& rkQ) const
	{
		return Quaternion(w+rkQ.w,x+rkQ.x,y+rkQ.y,z+rkQ.z);
	}
	//-----------------------------------------------------------------------
	Quaternion Quaternion::operator- (const Quaternion& q) const
	{
		return Quaternion( x - q.x , y - q.y, z - q.z, w - q.w);
	}

	Quaternion Quaternion::operator* (float value) const
	{
		return Quaternion(value*w,value*x,value*y,value*z);
	}

	Quaternion Quaternion::operator*( const Quaternion& q) const
	{
		return Quaternion(
			w * q.x + x * q.w + y * q.z - z * q.y
			,
			w * q.y + y * q.w + z * q.x - x * q.z
			,
			w * q.z + z * q.w + x * q.y - y * q.x
			,
			w * q.w - x * q.x - y * q.y - z * q.z
			);
	}
};

}

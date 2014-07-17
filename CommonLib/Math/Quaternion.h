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
		return Quaternion( q.x * x, q.y * y, q.z * z, q.w * w);
	}

// 	Quaternion Slerp( const Quaternion& q, float t)
// 	{
// 		return ;
// 	}
};

}

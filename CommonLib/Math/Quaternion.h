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
		float len = Length();
		float factor = 1.0f / len;
		x *= factor;
		y *= factor;
		z *= factor;
		w *= factor;
	}

	float Length()
	{
		 float len = sqrt(x * x + y * y + z * z + w * w);
		 return len;
	}

// algorithm is error on 3D Math book P147 the result is not the same with d3d
// 	Quaternion Multiply(const Quaternion& q)
// 	{
// 		float resultX = w * q.x + x * q.w + y * q.z - z * q.y;
// 		float resultY = w * q.y + y * q.w + z * q.x - x * q.z;
// 		float resultZ = w * q.z + z * q.w + x * q.y - y * q.x;
// 		float resultW = w * q.w - x * q.x - y * q.y - z * q.z;
// 
// 		return Quaternion( resultX, resultY, resultZ, resultW);
// 	}

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

// 	Quaternion Quaternion::operator*( const Quaternion& q) const
// 	{
// 		return Quaternion(
// 			w * q.x + x * q.w + y * q.z - z * q.y
// 			,
// 			w * q.y + y * q.w + z * q.x - x * q.z
// 			,
// 			w * q.z + z * q.w + x * q.y - y * q.x
// 			,
// 			w * q.w - x * q.x - y * q.y - z * q.z
// 			);
// 	}

	bool operator==(const Quaternion& q)
	{
		 if (q.x/q.w == x/w && q.y/q.w == y/w 
			 && q.z / q.w == z/w)
			 return true;

		 return false;
	}
};

}

#pragma once
#include <math.h>

namespace Dream
{

class Vector3
{
	Vector3(	
		float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	};

	~Vector3(){}

	float x,y,z;


	void Normalize()
	{
		float len = sqrt(x * x + y * y + z * z);
		float factor = 1.0f / len;
		x *= factor; y *= factor; z *= factor;
	}
};

class Vector4
{
public:
	Vector4(	
		float _x, float _y, float _z, float _w)
	{
		x = _x; y = _y; z = _z; w = _w;
	};
	~Vector4(){}

	float x,y,z,w;

	void Normalize()
	{
		float len = sqrt(x * x + y * y + z * z + w * w);
		float factor = 1.0f / len;
		x *= factor; y *= factor; z *= factor; w *= factor;
	}
};

}

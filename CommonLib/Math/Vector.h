#pragma once
#include <math.h>

namespace Dream
{

class Vector3
{
public:
	Vector3(	
		float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	};

	~Vector3(){}

	float x,y,z;


	void Normalize()
	{
		float len = Length();
		float factor = 1.0f / len;
		x *= factor; y *= factor; z *= factor;
	}

	float Length()
	{
		float len = sqrt(x * x + y * y + z * z);
		return len;
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
		float len = Length();
		float factor = 1.0f / len;
		x *= factor; y *= factor; z *= factor; w *= factor;
	}

	float Length()
	{
		float len = sqrt(x * x + y * y + z * z + w * w);
		return len;
	}
};

}

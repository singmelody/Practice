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

	float Dot(const Vector3& vec)
	{
		float value = vec.x * x + vec.y * y + vec.z * z;
		return value;
	}

	Vector3 Mid(const Vector3& vec)
	{
		return Vector3(
				(x + vec.x) * 0.5f,
				(y + vec.y) * 0.5f,
				(z + vec.z) * 0.5f);
	}

	Vector3 Cross(const Vector3& vec)
	{
		return Vector3(
				y * vec.z - z * vec.y,
				z * vec.x - x * vec.z,
				x * vec.y - y * vec.x
			);
	}

	bool operator==(const Vector3& vec)
	{
		if( vec.x == x && vec.y == y && vec.z == z)
			return true;

		return false;
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

	float Dot(const Vector4& vec)
	{
		float value = vec.x * x + vec.y * y + vec.z * z + vec.w * w;
		return value;
	}


};

}

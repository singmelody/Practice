#pragma once
#include "Vector.h"

namespace Dream
{

class Matrix
{
public:


	Matrix(){}
	Matrix(	
		float _m00, float _m01, float _m02, float _m03,
		float _m10, float _m11, float _m12, float _m13,
		float _m20, float _m21, float _m22, float _m23,
		float _m30, float _m31, float _m32, float _m33)
	{
		m[0][0] = _m00; m[0][1] = _m01; m[0][2] = _m02; m[0][3] = _m03;
		m[1][0] = _m10; m[1][1] = _m11; m[1][2] = _m12; m[1][3] = _m13;
		m[2][0] = _m20; m[2][1] = _m21; m[2][2] = _m22; m[2][3] = _m23;
		m[3][0] = _m30; m[3][1] = _m31; m[3][2] = _m32; m[3][3] = _m33;
	};
	~Matrix();

	float m[4][4];

	Matrix Transposed() const
	{
		return Matrix( m[0][0], m[1][0], m[2][0], m[3][0],
					   m[0][1], m[1][1], m[2][1], m[3][1],
					   m[0][2], m[1][2], m[2][2], m[3][2],
					   m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	void Transpose()
	{
		*this = Transposed();
	}

	Vector4 GetRow(int i)
	{
		return Vector4(m[i][0],m[i][1],m[i][2],m[i][3]);
	}

	Vector4 GetCol(int i)
	{
		return Vector4(m[0][i],m[1][i],m[2][i],m[3][i]);
	}

	Vector4 GetPosition()
	{
		return GetRow(3);
	}

	void SetScale(const Vector3& vec)
	{
		m[0][0] = vec.x;
		m[1][1] = vec.y;
		m[2][2] = vec.z;
	}

	void Identity()
	{
		m[0][0] = 1.0f; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
		m[1][0] = 0; m[1][1] = 1.0f; m[1][2] = 0; m[1][3] = 0;
		m[2][0] = 0; m[2][1] = 0; m[2][2] = 1.0f; m[2][3] = 0;
		m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1.0f;

	}

	void translation(const Vector3& vec)
	{
		m[3][0] = vec.x; m[3][1] = vec.y; m[3][2] = vec.z; 
	}
};

}

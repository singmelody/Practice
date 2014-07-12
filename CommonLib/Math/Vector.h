#pragma once

namespace Dream
{

class Vector3
{
	
}

class Vector4
{
public:
	Vector4(	
		float _m00, float _m01, float _m02, float _m03)
	{
		m[0][0] = _m11; m[0][1] = _m12; m[0][2] = _m13;
		m[1][0] = _m21; m[1][1] = _m22; m[1][2] = _m23;
		m[2][0] = _m31; m[2][1] = _m32; m[2][2] = _m33;
	};
	~Vector4();

	float m[4];

	Vector Transposed() const
	{
		return Vector( m[0][0], m[1][0], m[2][0], m[3][0],
					   m[0][1], m[1][1], m[2][1], m[3][1],
					   m[0][2], m[1][2], m[2][2], m[3][2],
					   m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	void Transpose()
	{
		*this = Transposed();
	}
};

}

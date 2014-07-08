#pragma once
#include "IShader.hpp"

namespace Dream
{
class Matrix;

class GlobalShader : public IShader
{
public:
	enum GMatrixParamIdx
	{
		// comment use, Global var begin
		eMatBegin = 0,
		eMatView  = eMatBegin,	//	g_View
		eMatProj,				//  g_LightView
		eMatEnd,

		// comment use, Local var begin
		eMatLocalBegin = eMatEnd,

		eMatLocalEnd,
	};

	void SetShaderParam( GMatrixParamIdx idx, const Matrix& mat);

	GlobalShader();
	virtual ~GlobalShader();

protected:
	virtual void SetShaderParam(const char* varName, void* data, PARAMTYPE type) {}

};

}

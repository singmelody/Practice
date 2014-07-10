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
	
		eMatCommonBegin = eMatBegin,
		eMatView  = eMatCommonBegin,	//	g_View
		eMatProj,	//  g_LightView
		eMatCommonEnd,

		// comment use, Local var begin
		eMatCustomBegin = eMatCommonEnd,
		eMatLocal =  eMatCustomBegin,
		eMatCustomEnd,

		eMatEnd = eMatCustomEnd
	};

	enum GVecParamIdx
	{
		eVecBegin = eMatEnd,

		eVecCommonBegin = eVecBegin,
		eVecAmbient = eVecCommonBegin,
		eVecCommonEnd,

		eVecCustomBeign = eVecCommonEnd,
		eVecCustomEnd,

		eVecEnd = eVecCustomEnd,
	};

	enum GTexParamIdx
	{
		eTexBegin = eVecEnd,

		eTexCommonBegin = eTexBegin,

		eTexCommonEnd,

		eTexCustomBegin = eTexCommonEnd,

		eTexCustomEnd,

		eTexEnd = eTexCustomEnd
	};

	void SetShaderParam( GMatrixParamIdx idx, const Matrix& mat);

	GlobalShader();
	virtual ~GlobalShader();

protected:
	virtual void SetShaderParam(const char* varName, void* data, PARAMTYPE type) {}

};

}

//***************************************************************************************
// Effects.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Defines lightweight effect wrappers to group an effect and its variables.
// Also defines a static Effects class from which we can access all of our effects.
//***************************************************************************************

#ifndef EFFECTS_H
#define EFFECTS_H

#include <d3d11.h>
#include <Inc/d3dx11effect.h>
#include <xnamath.h>
#include "LightHelper.h"
#include <string>

#pragma region Effect
class Effect
{
public:
	Effect(ID3DX11Effect* mf);
	virtual ~Effect();

private:
	Effect(const Effect& rhs);
	Effect& operator=(const Effect& rhs);

protected:
	ID3DX11Effect* mFX;
};
#pragma endregion

#pragma region TreeSpriteEffect
class TreeSpriteEffect : public Effect
{
public:
	TreeSpriteEffect(ID3DX11Effect* mf);
	~TreeSpriteEffect();

	void SetViewProj(CXMMATRIX M)                              { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                         { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFogColor(const FXMVECTOR v)                        { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f)                                  { FogStart->SetFloat(f); }
	void SetFogRange(float f)                                  { FogRange->SetFloat(f); }
	void SetDirLights(const DirectionalLight* lights)          { DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat)                      { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetTreeTextureMapArray(ID3D11ShaderResourceView* tex) { TreeTextureMapArray->SetResource(tex); }

	ID3DX11EffectTechnique* Light3Tech;
	ID3DX11EffectTechnique* Light3TexAlphaClipTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogTech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectShaderResourceVariable* TreeTextureMapArray;
};
#pragma endregion

#pragma region VecAddEffect
class VecAddEffect : public Effect
{
public:
	VecAddEffect(ID3DX11Effect* mf);
	~VecAddEffect();

	void SetInputA(ID3D11ShaderResourceView* srv)  { InputA->SetResource(srv); }
	void SetInputB(ID3D11ShaderResourceView* srv)  { InputB->SetResource(srv); }
	void SetOutput(ID3D11UnorderedAccessView* uav) { Output->SetUnorderedAccessView(uav); }

	ID3DX11EffectTechnique* VecAddTech;

	ID3DX11EffectShaderResourceVariable* InputA;
	ID3DX11EffectShaderResourceVariable* InputB;
	ID3DX11EffectUnorderedAccessViewVariable* Output;
};
#pragma endregion

#pragma region Effects
class Effects
{
public:
	static void InitAll();
	static void DestroyAll();
};
#pragma endregion

#endif // EFFECTS_H
//***************************************************************************************
// Effects.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************
#include <DXUT.h>
#include "Effects.h"

#pragma region Effect
Effect::Effect(ID3DX11Effect* mf)
	: mFX(mf)
{

}

Effect::~Effect()
{
//	ReleaseCOM(mFX);
}
#pragma endregion

#pragma region TreeSpriteEffect
TreeSpriteEffect::TreeSpriteEffect(ID3DX11Effect* mf)
	: Effect(mf)
{
	Light3Tech                = mFX->GetTechniqueByName("Light3");
	Light3TexAlphaClipTech    = mFX->GetTechniqueByName("Light3TexAlphaClip");
	Light3TexAlphaClipFogTech = mFX->GetTechniqueByName("Light3TexAlphaClipFog");

	ViewProj            = mFX->GetVariableByName("gViewProj")->AsMatrix();
	EyePosW             = mFX->GetVariableByName("gEyePosW")->AsVector();
	FogColor            = mFX->GetVariableByName("gFogColor")->AsVector();
	FogStart            = mFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange            = mFX->GetVariableByName("gFogRange")->AsScalar();
	DirLights           = mFX->GetVariableByName("gDirLights");
	Mat                 = mFX->GetVariableByName("gMaterial");
	TreeTextureMapArray = mFX->GetVariableByName("gTreeMapArray")->AsShaderResource();
}

TreeSpriteEffect::~TreeSpriteEffect()
{
}
#pragma endregion

#pragma region Effects

// BasicEffect*      Effects::BasicFX      = 0;
// TreeSpriteEffect* Effects::TreeSpriteFX = 0;

void Effects::InitAll()
{
// 	BasicFX = new BasicEffect(device, L"FX/Basic.fxo");
// 	TreeSpriteFX = new TreeSpriteEffect(device, L"FX/TreeSprite.fxo");
}

void Effects::DestroyAll()
{
// 	SafeDelete(BasicFX);
// 	SafeDelete(TreeSpriteFX);
}
#pragma endregion

#pragma region VecAddEffect


VecAddEffect::VecAddEffect(ID3DX11Effect* mf) 	
	: Effect(mf)
{
	VecAddTech  = mFX->GetTechniqueByName("VecAdd");

	InputA = mFX->GetVariableByName("gInputA")->AsShaderResource();
	InputB = mFX->GetVariableByName("gInputB")->AsShaderResource();
	Output = mFX->GetVariableByName("gOutput")->AsUnorderedAccessView();
}

VecAddEffect::~VecAddEffect()
{

}


#pragma endregion

#pragma region BlurEffect
BlurEffect::BlurEffect(ID3DX11Effect* mf) : Effect(mf)
{
	HorzBlurTech = mFX->GetTechniqueByName("HorzBlur");
	VertBlurTech = mFX->GetTechniqueByName("VertBlur");

	Weights     = mFX->GetVariableByName("gWeights")->AsScalar();
	InputMap    = mFX->GetVariableByName("gInput")->AsShaderResource();
	OutputMap   = mFX->GetVariableByName("gOutput")->AsUnorderedAccessView();
}

BlurEffect::~BlurEffect()
{
}
#pragma endregion

#pragma region BasicEffect
BasicEffect::BasicEffect(ID3DX11Effect* mf)
	: Effect(mf)
{
	Light1Tech    = mFX->GetTechniqueByName("Light1");
	Light2Tech    = mFX->GetTechniqueByName("Light2");
	Light3Tech    = mFX->GetTechniqueByName("Light3");

	Light0TexTech = mFX->GetTechniqueByName("Light0Tex");
	Light1TexTech = mFX->GetTechniqueByName("Light1Tex");
	Light2TexTech = mFX->GetTechniqueByName("Light2Tex");
	Light3TexTech = mFX->GetTechniqueByName("Light3Tex");

	Light0TexAlphaClipTech = mFX->GetTechniqueByName("Light0TexAlphaClip");
	Light1TexAlphaClipTech = mFX->GetTechniqueByName("Light1TexAlphaClip");
	Light2TexAlphaClipTech = mFX->GetTechniqueByName("Light2TexAlphaClip");
	Light3TexAlphaClipTech = mFX->GetTechniqueByName("Light3TexAlphaClip");

	Light1FogTech    = mFX->GetTechniqueByName("Light1Fog");
	Light2FogTech    = mFX->GetTechniqueByName("Light2Fog");
	Light3FogTech    = mFX->GetTechniqueByName("Light3Fog");

	Light0TexFogTech = mFX->GetTechniqueByName("Light0TexFog");
	Light1TexFogTech = mFX->GetTechniqueByName("Light1TexFog");
	Light2TexFogTech = mFX->GetTechniqueByName("Light2TexFog");
	Light3TexFogTech = mFX->GetTechniqueByName("Light3TexFog");

	Light0TexAlphaClipFogTech = mFX->GetTechniqueByName("Light0TexAlphaClipFog");
	Light1TexAlphaClipFogTech = mFX->GetTechniqueByName("Light1TexAlphaClipFog");
	Light2TexAlphaClipFogTech = mFX->GetTechniqueByName("Light2TexAlphaClipFog");
	Light3TexAlphaClipFogTech = mFX->GetTechniqueByName("Light3TexAlphaClipFog");

	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform      = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW           = mFX->GetVariableByName("gEyePosW")->AsVector();
	FogColor          = mFX->GetVariableByName("gFogColor")->AsVector();
	FogStart          = mFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange          = mFX->GetVariableByName("gFogRange")->AsScalar();
	DirLights         = mFX->GetVariableByName("gDirLights");
	Mat               = mFX->GetVariableByName("gMaterial");
	DiffuseMap        = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

BasicEffect::~BasicEffect()
{
}
#pragma endregion

#pragma region TessellationEffect
TessellationEffect::TessellationEffect(ID3DX11Effect* mf) : Effect(mf)
{
	TessTech = mFX->GetTechniqueByName("Tess");

	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform      = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW           = mFX->GetVariableByName("gEyePosW")->AsVector();
	FogColor          = mFX->GetVariableByName("gFogColor")->AsVector();
	FogStart          = mFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange          = mFX->GetVariableByName("gFogRange")->AsScalar();
	DirLights         = mFX->GetVariableByName("gDirLights");
	Mat               = mFX->GetVariableByName("gMaterial");
	DiffuseMap        = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

TessellationEffect::~TessellationEffect()
{
}
#pragma endregion

#pragma region BezierTessellationEffect
BezierTessellationEffect::BezierTessellationEffect(ID3DX11Effect* mf)
	: Effect(mf)
{
	TessTech = mFX->GetTechniqueByName("Tess");

	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform      = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW           = mFX->GetVariableByName("gEyePosW")->AsVector();
	FogColor          = mFX->GetVariableByName("gFogColor")->AsVector();
	FogStart          = mFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange          = mFX->GetVariableByName("gFogRange")->AsScalar();
	DirLights         = mFX->GetVariableByName("gDirLights");
	Mat               = mFX->GetVariableByName("gMaterial");
	DiffuseMap        = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

BezierTessellationEffect::~BezierTessellationEffect()
{
}
#pragma endregion
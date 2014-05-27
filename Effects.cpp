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
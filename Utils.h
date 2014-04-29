#pragma once

#include "DXUT.h"
#include "skinnedMesh.h"
#include "animation.h"

wchar_t *GetWC(const char *c);

#define PATH_TO_TEXTURES "meshes\\"
#define  CONTROLLER_NUM 1
#define HALF_PI		(0.5f * D3DX_PI)
#define FORTH_PI	(0.25f * D3DX_PI)

extern ID3DXEffect*		g_pEffect;       // D3DX effect interface
extern SkinnedMesh*		g_SkinnedMesh;
extern ID3DXLine*		g_Line;
extern Animation*		g_Anim;
extern int				g_activeAnimation;

enum PHYS_CONTRAINTS
{
	HINGE,
	TWISTCONE,
	BALLPOINT
};

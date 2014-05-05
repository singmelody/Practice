#pragma once

#include "DXUT.h"
#include "skinnedMesh.h"
#include "animation.h"
#include <string>

extern std::wstring GetWC(const char *c);
extern void LoadMesh(const char* c, ID3DXMesh** mesh);
extern void LoadTex(const char* c, IDirect3DTexture9** tex);

#define PATH_TO_TEXTURES "meshes\\"
#define  CONTROLLER_NUM 1
#define HALF_PI		(0.5f * D3DX_PI)
#define FORTH_PI	(0.25f * D3DX_PI)

class PhysicsManager;
class Morph;
class MultiMorph;
class MorphMesh;
class Face;

extern ID3DXEffect*		g_pEffect;       // D3DX effect interface
extern SkinnedMesh*		g_SkinnedMesh;
extern ID3DXLine*		g_Line;
extern Animation*		g_Anim;
extern int				g_activeAnimation;
extern PhysicsManager*	g_physicsEngine;
extern Morph*			g_Morph;
extern MultiMorph*		g_MultiMorph;
extern MorphMesh*		g_MorphSkeleton;
extern Face*			g_Face;

enum PHYS_CONTRAINTS
{
	HINGE,
	TWISTCONE,
	BALLPOINT
};

#pragma once

#include "DXUT.h"
#include "skinnedMesh.h"

wchar_t *GetWC(const char *c);

#define PATH_TO_TEXTURES "meshes\\"

extern ID3DXEffect* g_pEffect;       // D3DX effect interface
extern SkinnedMesh*	g_SkinnedMesh;

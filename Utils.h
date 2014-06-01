#pragma once

#include "DXUT.h"
#include "DXUTcamera.h"
#include "skinnedMesh.h"
#include "animation.h"
#include <string>
#include <Inc/d3dx11effect.h>
#include <d3d11.h>
#include <D3DX11tex.h>

extern std::wstring GetWC(const char *c);
extern void LoadMesh(const char* c, ID3DXMesh** mesh, ID3DXBuffer** ppAdjacency = NULL, ID3DXBuffer** ppMaterials = NULL, DWORD* materialCount = NULL);
extern void LoadMeshHierarchy(const char* c, const LPD3DXALLOCATEHIERARCHY& hier, D3DXFRAME** mesh);
extern void LoadTex(const char* c, IDirect3DTexture9** tex);
extern bool LoadShader(const char* c, ID3DX11Effect* &effect);

extern ID3D11ShaderResourceView* CreateTexture2DArraySRV(
	ID3D11Device* device, ID3D11DeviceContext* context,
	std::vector<std::string>& filenames,
	DXGI_FORMAT format = DXGI_FORMAT_FROM_FILE,
	UINT filter = D3DX11_FILTER_NONE, 
	UINT mipFilter = D3DX11_FILTER_LINEAR);

extern void PrintMeshDeclaration(ID3DXMesh* pMesh);

#define PATH_TO_TEXTURES "meshes\\"
#define  CONTROLLER_NUM 1
#define HALF_PI		(0.5f * D3DX_PI)
#define FORTH_PI	(0.25f * D3DX_PI)

class PhysicsManager;
class Morph;
class MultiMorph;
class MorphMesh;
class Face;
class ComplexFace;
class FaceModel;
class FaceController;
class FaceFacory;

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
extern FaceModel*		g_FaceModel;
extern FaceModel*		g_FaceModelGenerate;
extern FaceController*  g_FaceControllerGenerate;
extern ComplexFace*		g_ComplexFace;
extern D3DXVECTOR3		g_rayOrg;
extern D3DXVECTOR3		g_rayDir;	
extern FaceFacory*		g_pFaceFactory;


enum PHYS_CONTRAINTS
{
	HINGE,
	TWISTCONE,
	BALLPOINT
};

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
{                                                          \
	HRESULT hr = (x);                                      \
	if(FAILED(hr))                                         \
{                                                      \
	DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
	}                                                      \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

extern CModelViewerCamera          g_Camera;               // A model viewing camera
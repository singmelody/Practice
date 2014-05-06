#include "DXUT.h"
#include "SDKmisc.h"
#include "Utils.h"
#include <string>
#include "Morph.h"

std::wstring GetWC(const char *c)
{
	const size_t cSize = strlen(c)+1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs (wc, c, cSize);

	std::wstring wstr;
	wstr = wc;
	SAFE_DELETE(wc);

	return wstr;
}

void LoadMesh(const char* c, ID3DXMesh** mesh)
{
	std::wstring conv = GetWC(c);

	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str() );
	D3DXLoadMeshFromX(str, D3DXMESH_MANAGED, DXUTGetD3D9Device(), NULL, NULL, NULL, NULL, mesh);
}

void LoadTex(const char* c, IDirect3DTexture9** tex)
{
	std::wstring conv = GetWC(c);

	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str() );
	D3DXCreateTextureFromFile(DXUTGetD3D9Device(), str, tex);
}

extern void LoadMeshHierarchy(const char* c, const LPD3DXALLOCATEHIERARCHY& hier, D3DXFRAME** frame)
{
	std::wstring conv = GetWC(c);

	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str() );
	D3DXLoadMeshHierarchyFromX(str, D3DXMESH_MANAGED, DXUTGetD3D9Device(), hier, NULL, frame, NULL);
}

ID3DXEffect*    g_pEffect = NULL;       // D3DX effect interface
SkinnedMesh*	g_SkinnedMesh = NULL;
ID3DXLine*		g_Line = NULL;
Animation*		g_Anim = NULL;
PhysicsManager*	g_physicsEngine = NULL;
int				g_activeAnimation = 0;
Morph*			g_Morph = NULL;
MultiMorph*		g_MultiMorph = NULL;
MorphMesh*		g_MorphSkeleton = NULL;
Face*			g_Face = NULL;
ComplexFace*	g_ComplexFace = NULL;
FaceModel*		g_FaceModel = NULL;
FaceModel*		g_FaceModelGenerate = NULL;
FaceController* g_FaceControllerGenerate = NULL;
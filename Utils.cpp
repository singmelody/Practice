#include "DXUT.h"
#include "SDKmisc.h"
#include "Utils.h"
#include <string>
#include "Morph.h"
#include <iostream>
#include <D3DX11async.h>
#include "D3D11RenderDevice.h"

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

void LoadMesh(const char* c, ID3DXMesh** mesh, ID3DXBuffer** ppAdjacency, ID3DXBuffer** ppMaterials, DWORD* materialCount)
{
	std::wstring conv = GetWC(c);

	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str() );
	D3DXLoadMeshFromX(str, D3DXMESH_MANAGED, DXUTGetD3D9Device(), ppAdjacency, ppMaterials, NULL, materialCount, mesh);
}

void LoadTex(const char* c, IDirect3DTexture9** tex)
{
	std::wstring conv = GetWC(c);

	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str() );
	D3DXCreateTextureFromFile(DXUTGetD3D9Device(), str, tex);
}

extern bool LoadShader(const char* c, ID3DX11Effect** effect)
{
	std::wstring conv = GetWC(c);

	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str());
	
	DWORD compileFlag = NULL;

#if _DEBUG
	compileFlag |= D3D10_SHADER_DEBUG;
	compileFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	ID3D10Blob* compiledShader;
	ID3D10Blob* compiledError;
	HRESULT hr = D3DX11CompileFromFile( str, NULL, NULL, NULL, "fx_5_0", compileFlag, NULL, NULL, &compiledShader, &compiledError, NULL);

	if( compiledError != NULL || FAILED(hr))
	{
		SAFE_RELEASE(compiledError);
		return false;
	}

	D3DX11CreateEffectFromMemory( compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
		0, D3D11RenderDevice::Instance().m_d3d11Device, effect);

	SAFE_RELEASE(compiledShader);
	SAFE_RELEASE(compiledError);

	return true;
}


extern void LoadMeshHierarchy(const char* c, const LPD3DXALLOCATEHIERARCHY& hier, D3DXFRAME** frame)
{
	std::wstring conv = GetWC(c);

	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str() );
	D3DXLoadMeshHierarchyFromX(str, D3DXMESH_MANAGED, DXUTGetD3D9Device(), hier, NULL, frame, NULL);
}

void PrintMeshDeclaration(ID3DXMesh* pMesh)
{
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	pMesh->GetDeclaration(decl);

	for(int i=0; i<MAX_FVF_DECL_SIZE; i++)
	{
		if(decl[i].Type != D3DDECLTYPE_UNUSED)
		{
			std::cout << "Offset: " << (int)decl[i].Offset << ", Type: " << (int)decl[i].Type << ", Usage: " << (int)decl[i].Usage << "\n";
		}
		else break;
	}
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
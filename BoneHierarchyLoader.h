#pragma once

#include <d3dx9.h>

class BoneHierarchyLoader: public ID3DXAllocateHierarchy
{
public:
	BoneHierarchyLoader() :frameCount(0),meshCount(0){}

	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

	STDMETHOD(CreateMeshContainer)(THIS_ 
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData, 
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer);

	int frameCount;
	int meshCount;
};
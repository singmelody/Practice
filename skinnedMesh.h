#pragma once
#pragma warning(disable:4996)

#include <d3dx9.h>
#include <vector>

struct Bone: public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
};

struct BoneMesh: public D3DXMESHCONTAINER
{
	ID3DXMesh* OriginalMesh;
	std::vector<D3DMATERIAL9> materials;
	std::vector<IDirect3DTexture9*> textures;

	DWORD NumAttributeGroups;
	D3DXATTRIBUTERANGE* attributeTable;
	D3DXMATRIX** boneMatrixPtrs;
	D3DXMATRIX* boneOffsetMatrices;
	D3DXMATRIX* currentBoneMatrices;
};

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


class SkinnedMesh
{
public:
	SkinnedMesh();
	~SkinnedMesh();
	void Load(WCHAR* fileName);
	void Render(Bone *bone);
	void RenderSkeleton(Bone* bone, Bone *parent, D3DXMATRIX world);

private:		
	void UpdateMatrices(Bone* bone, D3DXMATRIX *parentMatrix);
	void SetupBoneMatrixPointers(Bone *bone);

	D3DXFRAME *m_pRootBone;
	LPD3DXMESH m_pSphereMesh;


};

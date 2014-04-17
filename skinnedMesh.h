#pragma once
#pragma warning(disable:4996)

#include <d3dx9.h>
#include <vector>

struct Bone: public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;

	Bone()
	{
		Name = NULL;
		pMeshContainer = NULL;
		pFrameSibling = NULL;
		pFrameFirstChild = NULL;
	}
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

	BoneMesh()
	{
		OriginalMesh = NULL;
		NumAttributeGroups = NULL;
		attributeTable = NULL;
		boneMatrixPtrs = NULL;
		boneOffsetMatrices = NULL;
		currentBoneMatrices = NULL;

		// base
		Name = NULL;
		pMaterials = NULL;
		pEffects = NULL;
		NumMaterials = NULL;
		pAdjacency = NULL;
		pSkinInfo = NULL;
	}


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

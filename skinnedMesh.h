#pragma once
#pragma warning(disable:4996)

#include <d3dx9.h>
#include <vector>
#include "BoneHierarchyLoader.h"
#include "OBB.h"
//#define SOFT 1

struct VERTEX{
	VERTEX();
	VERTEX(D3DXVECTOR3 pos, D3DCOLOR col){position = pos; color = col;}
	D3DXVECTOR3 position;
	D3DCOLOR color;
	static const DWORD FVF;
};

struct DeclVertex{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
	static const DWORD FVF;
};

struct LineVertex{
	LineVertex();
	LineVertex(D3DXVECTOR3 pos, D3DCOLOR col){position = pos; color = col;}
	D3DXVECTOR3 position;
	D3DCOLOR color;
	static const DWORD FVF;
};

struct Bone: public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
	// ptr to an Obb
	OBB *m_Obb;

	// bone's pivot point (offset from obb center)
	D3DXVECTOR3 m_pivot;

	// Original orientation of this bone
	D3DXQUATERNION m_originalRot;

	Bone()
	{
		Name = NULL;
		pMeshContainer = NULL;
		pFrameSibling = NULL;
		pFrameFirstChild = NULL;
		m_Obb = NULL;
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

	D3DXINTERSECTINFO GetFace(D3DXVECTOR3 &rayOrg, D3DXVECTOR3 &rayDir);
	ID3DXMesh* GetHitMesh(D3DXVECTOR3 &rayOrg, D3DXVECTOR3 &rayDir);
};

class SkinnedMesh
{
public:
	SkinnedMesh();
	virtual ~SkinnedMesh();
	void Load(WCHAR* fileName);
	void RenderSoft(Bone *bone, const char* animTech, const char* staticTech, bool shadow = false);
	void RenderHAL(Bone* bone,const char* animTech, const char* staticTech, bool shadow = false);
	void RenderSkeleton(Bone* bone, Bone *parent, D3DXMATRIX world);

	void SetShadowMatrix(D3DXMATRIX& matrix) { m_shadow = matrix; }

	void SetPose(D3DXMATRIX world);
	void SetAnimation(const std::string& name);
	void GetAnimations();

	const std::vector<std::string>& GetAnimationNames();
	ID3DXAnimationController* GetController();

	D3DXFRAME* GetBone(const char* name);

	void UpdateMatrices(Bone* bone, D3DXMATRIX *parentMatrix);
protected:
	D3DXFRAME *m_pRootBone;

private:		
	void SetupBoneMatrixPointers(Bone *bone);

	LPD3DXMESH m_pSphereMesh;
	ID3DXAnimationController* m_controller;
	std::vector<std::string> m_animations;

#ifdef SOFT
	BoneHierarchyLoaderSoft m_boneHierarchy;
#else 
	BoneHierarchyLoaderHAL m_boneHierarchy;
#endif

	D3DXMATRIX m_shadow;
};

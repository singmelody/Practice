#include "DXUT.h"
#include "skinnedMesh.h"
#include "SDKmisc.h"
#include <Windows.h>
#include "Utils.h"

HRESULT BoneHierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	frameCount++;
	Bone *newBone = new Bone;
	memset(newBone, 0, sizeof(Bone));

	//Copy name
	if(Name != NULL)
	{
		newBone->Name = new char[strlen(Name)+1];
		strcpy(newBone->Name, Name);
	}

	//Set the transformation matrices
	D3DXMatrixIdentity(&newBone->TransformationMatrix);
	D3DXMatrixIdentity(&newBone->CombinedTransformationMatrix);

	//Return the new bone...
	*ppNewFrame = (D3DXFRAME*)newBone;

	return S_OK;
}

HRESULT BoneHierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
	if(!pFrameToFree)
		return S_OK;

	if(pFrameToFree)
	{
		//Free Name String
		if(pFrameToFree->Name != NULL)
			SAFE_DELETE_ARRAY(pFrameToFree->Name);

		if(pFrameToFree->pMeshContainer != NULL)
			DestroyMeshContainer(( ((Bone*)pFrameToFree)->pMeshContainer ));
	}

	if(pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if(pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	SAFE_DELETE(pFrameToFree);

	return S_OK; 
}

HRESULT BoneHierarchyLoader::CreateMeshContainer(LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	meshCount++;
	//Just return a temporary mesh for now...
	BoneMesh* boneMesh = new BoneMesh;
	memset( boneMesh, 0, sizeof(BoneMesh));

	// Get Mesh Data
	boneMesh->OriginalMesh = pMeshData->pMesh;
	boneMesh->MeshData.pMesh = pMeshData->pMesh;
	boneMesh->MeshData.Type	= pMeshData->Type;

	pMeshData->pMesh->AddRef();

	for (int i = 0; i < NumMaterials; ++i)
	{
		D3DXMATERIAL mtrl;
		memcpy(&mtrl, &pMaterials[i], sizeof(D3DXMATERIAL));
		boneMesh->materials.push_back(mtrl.MatD3D);

		IDirect3DTexture9* itexture = NULL;
		if(mtrl.pTextureFilename != NULL)
		{
			char textureFileName[90];
			strcpy(textureFileName, PATH_TO_TEXTURES);
			strcat(textureFileName, mtrl.pTextureFilename);
			const wchar_t* conv = GetWC(textureFileName);
			
			WCHAR str[MAX_PATH];
			HRESULT hr;
			V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv ) );
			SAFE_DELETE(conv);

			D3DXCreateTextureFromFile( DXUTGetD3D9Device(), str, &itexture);
		}

		boneMesh->textures.push_back((IDirect3DTexture9*)itexture);
	}

	if ( pSkinInfo )
	{
		boneMesh->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();	//Add reference so that the SkinInfo isnt deallocated

		//Clone mesh and store in boneMesh->MeshData.pMesh
		pMeshData->pMesh->CloneMeshFVF( D3DXMESH_MANAGED, pMeshData->pMesh->GetFVF(),
			DXUTGetD3D9Device(), &boneMesh->MeshData.pMesh);

		// Get Atr Table
		boneMesh->MeshData.pMesh->GetAttributeTable( NULL, &boneMesh->NumAttributeGroups);
		boneMesh->attributeTable = new D3DXATTRIBUTERANGE[boneMesh->NumAttributeGroups];
		boneMesh->MeshData.pMesh->GetAttributeTable( boneMesh->attributeTable, NULL);

		// Create bone offset and current matrices
		int boneNum = pSkinInfo->GetNumBones();
		boneMesh->boneOffsetMatrices = new D3DXMATRIX[boneNum];
		boneMesh->currentBoneMatrices = new D3DXMATRIX[boneNum];

		for (int i = 0; i < boneNum; ++i)
		{
			boneMesh->boneOffsetMatrices[i] = *(boneMesh->pSkinInfo->GetBoneOffsetMatrix(i));
		}
	}

	*ppNewMeshContainer = boneMesh;

	return S_OK;
}

HRESULT BoneHierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	BoneMesh* boneMesh = static_cast<BoneMesh*>(pMeshContainerBase);

	if(pMeshContainerBase != NULL)
	{	
		for (int i = 0; i < boneMesh->textures.size(); ++i)
			SAFE_RELEASE( boneMesh->textures[i] );

		boneMesh->textures.clear();
		boneMesh->materials.clear();
	}

	if(boneMesh->pSkinInfo)
	{
		SAFE_RELEASE(boneMesh->pSkinInfo);
		SAFE_RELEASE(boneMesh->MeshData.pMesh);
	}

	if(boneMesh->OriginalMesh)
		SAFE_RELEASE(boneMesh->OriginalMesh);

	if(boneMesh->attributeTable)
		SAFE_DELETE_ARRAY( boneMesh->attributeTable );

	if(boneMesh->boneMatrixPtrs)
		SAFE_DELETE_ARRAY(boneMesh->boneMatrixPtrs);

	if(boneMesh->boneOffsetMatrices)
		SAFE_DELETE_ARRAY(boneMesh->boneOffsetMatrices);

	if(boneMesh->currentBoneMatrices)
		SAFE_DELETE_ARRAY(boneMesh->currentBoneMatrices);

	SAFE_DELETE( boneMesh );

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//									SKINNED MESH												//
//////////////////////////////////////////////////////////////////////////////////////////////////

struct VERTEX{
	VERTEX();
	VERTEX(D3DXVECTOR3 pos, D3DCOLOR col){position = pos; color = col;}
	D3DXVECTOR3 position;
	D3DCOLOR color;
	static const DWORD FVF;
};

const DWORD VERTEX::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

SkinnedMesh::SkinnedMesh()
{
	m_pRootBone = NULL;
	m_pSphereMesh = NULL;
}

SkinnedMesh::~SkinnedMesh()
{
	BoneHierarchyLoader boneHierarchy;
	boneHierarchy.DestroyFrame(m_pRootBone);

	SAFE_RELEASE( m_pSphereMesh );
}

void SkinnedMesh::Load(WCHAR* fileName)
{
	BoneHierarchyLoader boneHierarchy;
    HRESULT hr;

	WCHAR str[MAX_PATH];
	V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, fileName ) );

	//Load a bone hierarchy from a file
	D3DXLoadMeshHierarchyFromX(str, D3DXMESH_MANAGED, DXUTGetD3D9Device(), &boneHierarchy,
		NULL, &m_pRootBone, NULL);

	SetupBoneMatrixPointers((Bone*)m_pRootBone);

	//Update Bone transformation matrices
	D3DXMATRIX i;
	D3DXMatrixIdentity(&i);
	UpdateMatrices((Bone*)m_pRootBone, &i);

	//Create Sphere
	D3DXCreateSphere(DXUTGetD3D9Device(), 0.02f, 10, 10, &m_pSphereMesh, NULL);
}


void SkinnedMesh::UpdateMatrices(Bone* bone, D3DXMATRIX *parentMatrix)
{
	if(bone == NULL)return;

	//Calculate the combined transformation matrix
	D3DXMatrixMultiply(&bone->CombinedTransformationMatrix,
		&bone->TransformationMatrix,
		parentMatrix);

	//Perform the same calculation on all siblings
	if(bone->pFrameSibling)
	{
		UpdateMatrices((Bone*)bone->pFrameSibling, 
			parentMatrix);
	}

	//... and all children
	if(bone->pFrameFirstChild)
	{
		UpdateMatrices((Bone*)bone->pFrameFirstChild, 
			&bone->CombinedTransformationMatrix);
	}
}

void SkinnedMesh::RenderSkeleton(Bone* bone, Bone *parent, D3DXMATRIX world)
{
	static int count = 0;
	count++;
	//Temporary function to render the bony hierarchy
	if(bone == NULL)bone = (Bone*)m_pRootBone;

	D3DXMATRIX r, s;
	D3DXMatrixRotationYawPitchRoll(&r, -D3DX_PI * 0.5f, 0.0f, 0.0f);

	//Draw line between bones
	if(parent != NULL && bone->Name != NULL && parent->Name != NULL)
	{
		//Draw Sphere
		DXUTGetD3D9Device()->SetRenderState(D3DRS_LIGHTING, true);
		DXUTGetD3D9Device()->SetTransform(D3DTS_WORLD, &(r * bone->CombinedTransformationMatrix * world));
		m_pSphereMesh->DrawSubset(0);

		D3DXMATRIX w1 = bone->CombinedTransformationMatrix;
		D3DXMATRIX w2 = parent->CombinedTransformationMatrix;

		//Extract translation
		D3DXVECTOR3 thisBone = D3DXVECTOR3(w1(3, 0), w1(3, 1), w1(3, 2));
		D3DXVECTOR3 ParentBone = D3DXVECTOR3(w2(3, 0), w2(3, 1), w2(3, 2));

		if(D3DXVec3Length(&(thisBone - ParentBone)) < 2.0f)
		{
			DXUTGetD3D9Device()->SetTransform(D3DTS_WORLD, &world);
			VERTEX vert[] = {VERTEX(ParentBone, 0xffff0000), VERTEX(thisBone, 0xff00ff00)};
			DXUTGetD3D9Device()->SetRenderState(D3DRS_LIGHTING, false);
			DXUTGetD3D9Device()->SetFVF(VERTEX::FVF);
			DXUTGetD3D9Device()->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &vert[0], sizeof(VERTEX));
		}
	}

	if(bone->pFrameSibling)RenderSkeleton((Bone*)bone->pFrameSibling, parent, world);
	if(bone->pFrameFirstChild)RenderSkeleton((Bone*)bone->pFrameFirstChild, bone, world);
}

void SkinnedMesh::SetupBoneMatrixPointers(Bone *bone)
{
	if(bone->pMeshContainer != NULL)
	{
		BoneMesh* boneMesh = (BoneMesh*)bone->pMeshContainer;
		
		if(boneMesh->pSkinInfo != NULL)
		{
			int numBones = boneMesh->pSkinInfo->GetNumBones();

			boneMesh->boneMatrixPtrs = new D3DXMATRIX*[numBones];

			for (int i = 0; i < numBones; ++i)
			{
				Bone* b = (Bone*)D3DXFrameFind( m_pRootBone, 
					boneMesh->pSkinInfo->GetBoneName(i));

				if(b)
				{
					boneMesh->boneMatrixPtrs[i] = &b->CombinedTransformationMatrix;
				}
				else
				{
					boneMesh->boneMatrixPtrs[i] = NULL;
				}
			}
		}
	}

	if(bone->pFrameSibling)
		SetupBoneMatrixPointers( (Bone*)bone->pFrameSibling );

	if(bone->pFrameFirstChild)
		SetupBoneMatrixPointers( (Bone*)bone->pFrameFirstChild );

}

void SkinnedMesh::Render(Bone *bone)
{
	if(bone == NULL)
		bone = (Bone *)m_pRootBone;

	if (bone->pMeshContainer != NULL)
	{ 
		BoneMesh* boneMesh = (BoneMesh*)bone->pMeshContainer;

		if(boneMesh->pSkinInfo != NULL)
		{
			int numBones = boneMesh->pSkinInfo->GetNumBones();

			for (int i = 0; i < numBones; ++i)
			{
				D3DXMatrixMultiply( &boneMesh->currentBoneMatrices[i],
					&boneMesh->boneOffsetMatrices[i],
					boneMesh->boneMatrixPtrs[i]);
			}

			// Update the skinned mesh
			BYTE* src = NULL, *dest = NULL;
			boneMesh->OriginalMesh->LockVertexBuffer( D3DLOCK_READONLY, (VOID**)&src);
			boneMesh->MeshData.pMesh->LockVertexBuffer( 0, (VOID**)&dest);

			boneMesh->pSkinInfo->UpdateSkinnedMesh( boneMesh->currentBoneMatrices, NULL, src, dest);

			boneMesh->MeshData.pMesh->UnlockVertexBuffer();
			boneMesh->OriginalMesh->UnlockVertexBuffer();
		

			// Render core
			for (int i = 0; i < boneMesh->NumAttributeGroups; ++i)
			{
				int mtrlIndex = boneMesh->attributeTable[i].AttribId;
				DXUTGetD3D9Device()->SetMaterial(&(boneMesh->materials[mtrlIndex]));
				DXUTGetD3D9Device()->SetTexture( 0, boneMesh->textures[i]);
				boneMesh->MeshData.pMesh->DrawSubset(mtrlIndex);
			}
		}
	}

	if(bone->pFrameSibling)
		Render((Bone*)bone->pFrameSibling);

	if(bone->pFrameFirstChild)
		Render((Bone*)bone->pFrameFirstChild);
}

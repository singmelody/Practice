#include "DXUT.h"
#include "SDKmisc.h"
#include "skinnedMesh.h"
#include <Windows.h>
#include "Utils.h"

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
	m_boneHierarchy.DestroyFrame(m_pRootBone);

	SAFE_RELEASE( m_pSphereMesh );
}

void SkinnedMesh::Load(WCHAR* fileName)
{
    HRESULT hr;

	WCHAR str[MAX_PATH];
	V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, fileName ) );

	//Load a bone hierarchy from a file
	D3DXLoadMeshHierarchyFromX(str, D3DXMESH_MANAGED, DXUTGetD3D9Device(), &m_boneHierarchy,
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
		else
		{

		}
	}

	if(bone->pFrameSibling)
		SetupBoneMatrixPointers( (Bone*)bone->pFrameSibling );

	if(bone->pFrameFirstChild)
		SetupBoneMatrixPointers( (Bone*)bone->pFrameFirstChild );

}

void SkinnedMesh::RenderSoft(Bone *bone, const char* animTech, const char* staticTech)
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
			for (size_t i = 0; i < boneMesh->NumAttributeGroups; ++i)
			{
				int mtrlIndex = boneMesh->attributeTable[i].AttribId;
				DXUTGetD3D9Device()->SetMaterial(&(boneMesh->materials[mtrlIndex]));
				DXUTGetD3D9Device()->SetTexture( 0, boneMesh->textures[i]);

				g_pEffect->SetTexture( "g_MeshTexture",  boneMesh->textures[mtrlIndex]);

				UINT iPass, cPasses;
				g_pEffect->Begin( &cPasses, NULL);

				for (iPass = 0; iPass < cPasses; ++iPass)
				{
					g_pEffect->BeginPass(iPass);

					boneMesh->MeshData.pMesh->DrawSubset(mtrlIndex);

					g_pEffect->EndPass();

				}

				g_pEffect->End();
			}
		}
	}

	if(bone->pFrameSibling)
		RenderSoft((Bone*)bone->pFrameSibling, animTech, staticTech);

	if(bone->pFrameFirstChild)
		RenderSoft((Bone*)bone->pFrameFirstChild, animTech, staticTech);
}

void SkinnedMesh::RenderHAL(Bone* bone, const char* animTech, const char* staticTech)
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

			g_pEffect->SetMatrixArray( 
				"MatrixPalette", boneMesh->currentBoneMatrices, numBones);

			D3DXHANDLE hTech = g_pEffect->GetTechniqueByName("SkinHAL");
			g_pEffect->SetTechnique(hTech);

			// set world
			D3DXMATRIX identity;
			D3DXMatrixIdentity(&identity);

			g_pEffect->SetMatrix( "g_mWorld", &identity);

			// Render core
			for (size_t i = 0; i < boneMesh->NumAttributeGroups; ++i)
			{
				int mtrlIndex = boneMesh->attributeTable[i].AttribId;

				DXUTGetD3D9Device()->SetMaterial(&(boneMesh->materials[mtrlIndex]));
				DXUTGetD3D9Device()->SetTexture( 0, boneMesh->textures[mtrlIndex]);

				g_pEffect->SetTexture( "g_MeshTexture",  boneMesh->textures[mtrlIndex]);

				g_pEffect->Begin( NULL, NULL);
				g_pEffect->BeginPass(0);

				boneMesh->MeshData.pMesh->DrawSubset(mtrlIndex);

				g_pEffect->EndPass();
				g_pEffect->End();
			}
		}
		else
		{
			g_pEffect->SetMatrix( "g_mWorld", &bone->CombinedTransformationMatrix);

			D3DXHANDLE hTech = g_pEffect->GetTechniqueByName("RenderSceneWithTexture1Light");
			g_pEffect->SetTechnique(hTech);

			//Render the mesh
			int numMaterials = (int)boneMesh->materials.size();

			for (int i = 0; i < numMaterials; ++i)
			{
				DXUTGetD3D9Device()->SetMaterial(&boneMesh->materials[i]);
				g_pEffect->SetTexture( "g_MeshTexture", boneMesh->textures[i]);

				g_pEffect->Begin( NULL, NULL);
				g_pEffect->BeginPass(0);

				boneMesh->MeshData.pMesh->DrawSubset(i);

				g_pEffect->EndPass();
				g_pEffect->End();
			}
		}
	}

	if(bone->pFrameSibling)
		RenderHAL((Bone*)bone->pFrameSibling, animTech, staticTech);

	if(bone->pFrameFirstChild)
		RenderHAL((Bone*)bone->pFrameFirstChild, animTech, staticTech);
}

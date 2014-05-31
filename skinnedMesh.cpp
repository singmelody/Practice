#include "DXUT.h"
#include "SDKmisc.h"
#include "skinnedMesh.h"
#include <Windows.h>
#include "Utils.h"
#include <map>
#include <queue>
#include "CharacterDecal.h"
//////////////////////////////////////////////////////////////////////////////////////////////////
//									SKINNED MESH												//
//////////////////////////////////////////////////////////////////////////////////////////////////


const DWORD LineVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
const DWORD VERTEX::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
const DWORD SimpleDeclVertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

SkinnedMesh::SkinnedMesh()
{
	m_pRootBone = NULL;
	m_pSphereMesh = NULL;
	m_controller = NULL;
}

SkinnedMesh::~SkinnedMesh()
{
	m_animations.clear();

	m_boneHierarchy.DestroyFrame(m_pRootBone);

	SAFE_RELEASE( m_pSphereMesh );
	SAFE_RELEASE( m_controller );
}

void SkinnedMesh::Load(WCHAR* fileName)
{
    HRESULT hr;

	WCHAR str[MAX_PATH];
	V( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, fileName ) );

	//Load a bone hierarchy from a file
	D3DXLoadMeshHierarchyFromX(str, D3DXMESH_MANAGED, DXUTGetD3D9Device(), &m_boneHierarchy,
		NULL, &m_pRootBone, &m_controller);

	SetupBoneMatrixPointers((Bone*)m_pRootBone);

	//Update Bone transformation matrices
	D3DXMATRIX i;
	D3DXMatrixIdentity(&i);
	UpdateMatrices((Bone*)m_pRootBone, &i);

	//Create Sphere
	D3DXCreateSphere(DXUTGetD3D9Device(), 0.02f, 10, 10, &m_pSphereMesh, NULL);

	GetAnimations();
}

const std::vector<std::string>& SkinnedMesh::GetAnimationNames()
{
	return m_animations;
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

		float length = D3DXVec3Length(&(thisBone - ParentBone));
		if( length < 2.0f)
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

void SkinnedMesh::RenderSoft(Bone *bone, const char* animTech, const char* staticTech, bool shadow)
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

				D3DXHANDLE hTech = g_pEffect->GetTechniqueByName(animTech);
				g_pEffect->SetTechnique(hTech);

				if(!shadow)
				{
					D3DXMATRIX identity;
					D3DXMatrixIdentity(&identity);
					g_pEffect->SetMatrix( "g_mWorld", &identity);
				}
				else
				{
					g_pEffect->SetMatrix( "g_mWorld", &m_shadow);
				}

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
		else
		{
			if(!shadow)
			{
				g_pEffect->SetMatrix( "g_mWorld", &bone->CombinedTransformationMatrix);
			}
			else
			{
				D3DXMATRIX result;
				D3DXMatrixMultiply( &result, &bone->CombinedTransformationMatrix, &m_shadow);
				g_pEffect->SetMatrix( "g_mWorld", &result);
			}

			D3DXHANDLE hTech = g_pEffect->GetTechniqueByName(staticTech);
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
		RenderSoft((Bone*)bone->pFrameSibling, animTech, staticTech, shadow);

	if(bone->pFrameFirstChild)
		RenderSoft((Bone*)bone->pFrameFirstChild, animTech, staticTech, shadow);
}

void SkinnedMesh::RenderHAL(Bone* bone, const char* animTech, const char* staticTech, bool shadow)
{
	if(bone == NULL)
		bone = (Bone *)m_pRootBone;

	if (bone->pMeshContainer != NULL)
	{ 
		BoneMesh* boneMesh = (BoneMesh*)bone->pMeshContainer;

// 		ID3DXMesh* hitMesh = boneMesh->GetHitMesh(g_rayOrg, g_rayDir);
// 
// 		//Temp rendering of original mesh and decal mesh
// 		if(boneMesh->pSkinInfo != NULL)
// 		{
// 			bool isHead = strcmp(bone->Name, "Face") == 0;
// 
// 			D3DXMATRIX W;
// 			D3DXMatrixTranslation(&W, -1.0f, isHead ? 1.63f : 0.0f, 0.0f);
// 			g_pEffect->SetMatrix("g_mWorld", &W);
// 			g_pEffect->SetTexture("g_MeshTexture", boneMesh->textures[0]);
// 
// 			D3DXHANDLE hTech = g_pEffect->GetTechniqueByName("SimpleDecal");
// 			g_pEffect->SetTechnique(hTech);
// 			g_pEffect->Begin(NULL, NULL);
// 			g_pEffect->BeginPass(0);			
// 
// 			if(hitMesh != NULL)
// 			{
// 				hitMesh->DrawSubset(0);
// 				hitMesh->Release();
// 			}
// 
// 			//Render original mesh as wireframe
// 			DXUTGetD3D9Device()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
// 			g_pEffect->CommitChanges();
// 			boneMesh->OriginalMesh->DrawSubset(0);
// 
// 			g_pEffect->EndPass();
// 			g_pEffect->End();			
// 
// 			//restore renderstates
// 			DXUTGetD3D9Device()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
// 			DXUTGetD3D9Device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
// 		}

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

			D3DXHANDLE hTech = g_pEffect->GetTechniqueByName(animTech);
			g_pEffect->SetTechnique(hTech);

			if(!shadow)
			{
				D3DXMATRIX identity;
				D3DXMatrixIdentity(&identity);
				g_pEffect->SetMatrix( "g_mWorld", &identity);
			}
			else
			{
				g_pEffect->SetMatrix( "g_mWorld", &m_shadow);
			}

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

				//Also render all character decals with the same bone transforms
				for(int cd=0; cd<(int)boneMesh->m_decals.size(); cd++)
				{
					boneMesh->m_decals[cd]->Render();
				}
			}
		}
		else
		{
			if(!shadow)
			{
				g_pEffect->SetMatrix( "g_mWorld", &bone->CombinedTransformationMatrix);
			}
			else
			{
				D3DXMATRIX result;
				D3DXMatrixMultiply( &result, &bone->CombinedTransformationMatrix, &m_shadow);
				g_pEffect->SetMatrix( "g_mWorld", &result);
			}

			D3DXHANDLE hTech = g_pEffect->GetTechniqueByName(staticTech);
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
		RenderHAL((Bone*)bone->pFrameSibling, animTech, staticTech, shadow);

	if(bone->pFrameFirstChild)
		RenderHAL((Bone*)bone->pFrameFirstChild, animTech, staticTech, shadow);
}


void SkinnedMesh::GetAnimations()
{
	ID3DXAnimationSet* anim = NULL;
	for (int i = 0; i < m_controller->GetMaxNumAnimationSets(); ++i)
	{
		anim = NULL;
		m_controller->GetAnimationSet( i, &anim);
		if( anim != NULL)
		{
			m_animations.push_back(anim->GetName());
			SAFE_RELEASE(anim);
		}
	}
}

void SkinnedMesh::SetPose(D3DXMATRIX world)
{
	UpdateMatrices( (Bone*)m_pRootBone, &world);
}

void SkinnedMesh::SetAnimation(const std::string& name)
{
	ID3DXAnimationSet* anim = NULL;

	int numAnims = (int)m_controller->GetMaxNumAnimationSets();

	for (int i = 0; i < numAnims; ++i)
	{
		anim = NULL;
		m_controller->GetAnimationSet( i, &anim);

		if( anim != NULL )
		{
			if( strcmp(name.c_str(), anim->GetName()) == 0 )
				m_controller->SetTrackAnimationSet( 0, anim);

			anim->Release();
		}
	}
}

ID3DXAnimationController* SkinnedMesh::GetController()
{
	ID3DXAnimationController* newAnimController = NULL;

	if ( m_controller != NULL )
	{
		m_controller->CloneAnimationController(
			m_controller->GetMaxNumAnimationOutputs(),
			m_controller->GetMaxNumAnimationSets(),
			m_controller->GetMaxNumTracks(),
			m_controller->GetMaxNumEvents(),
			&newAnimController
			);
	}

	return newAnimController;
}

D3DXFRAME* SkinnedMesh::GetBone(const char* name)
{
	return D3DXFrameFind( m_pRootBone, name);
}

void SkinnedMesh::AddDecal(Bone* pBone /*= NULL*/)
{
	if(pBone == NULL)
		pBone = (Bone*)m_pRootBone;

	//Attempt to add decal
	if(pBone->pMeshContainer != NULL)
	{
		BoneMesh* boneMesh = (BoneMesh*)pBone->pMeshContainer;
		boneMesh->AddDecal(g_rayOrg, g_rayDir, 0.1f);
	}

	if(pBone->pFrameSibling != NULL)		
		AddDecal((Bone*)pBone->pFrameSibling);

	if(pBone->pFrameFirstChild != NULL)
		AddDecal((Bone*)pBone->pFrameFirstChild);
}

D3DXINTERSECTINFO BoneMesh::GetFace(D3DXVECTOR3 &rayOrg, D3DXVECTOR3 &rayDir)
{
	D3DXINTERSECTINFO hitInfo;

	// Must test against software skinned model
	if( pSkinInfo != NULL )
	{
		// make sure vertex format is correct
		if(OriginalMesh->GetFVF() != SimpleDeclVertex::FVF )
		{
			hitInfo.FaceIndex = 0xffffffff;
			return hitInfo;
		}

		// set up bone transforms
		int numBones = pSkinInfo->GetNumBones();
		for (int i = 0; i < numBones; ++i)
		{
			D3DXMatrixMultiply(&currentBoneMatrices[i],
				&boneOffsetMatrices[i], 
				boneMatrixPtrs[i]);
		}

		//Create temp mesh
		ID3DXMesh *tempMesh = NULL;
		OriginalMesh->CloneMeshFVF(D3DXMESH_MANAGED, 
			OriginalMesh->GetFVF(), 
			DXUTGetD3D9Device(), 
			&tempMesh);

		//get source and destination buffer
		BYTE *src = NULL;
		BYTE *dest = NULL;
		OriginalMesh->LockVertexBuffer(D3DLOCK_READONLY, (VOID**)&src);		
		tempMesh->LockVertexBuffer(0, (VOID**)&dest);

		//Perform the software skinning
		pSkinInfo->UpdateSkinnedMesh(currentBoneMatrices, NULL, src, dest);

		//unlock buffers
		OriginalMesh->UnlockVertexBuffer();
		tempMesh->UnlockVertexBuffer();

		//Perform the intersection test
		BOOL Hit;
		D3DXIntersect(tempMesh, 
			&rayOrg, 
			&rayDir, 
			&Hit, 
			&hitInfo.FaceIndex, 
			&hitInfo.U, 
			&hitInfo.V, 
			&hitInfo.Dist, 
			NULL, 
			NULL);

		//Release temporary mesh
		tempMesh->Release();

		if(Hit)
		{
			//Successful hit
			return hitInfo;
		}
	}

	//No Hit
	hitInfo.FaceIndex = 0xffffffff;
	hitInfo.Dist = -1.0f;
	return hitInfo;	
}

ID3DXMesh* BoneMesh::CreateDecalMesh(D3DXVECTOR3 &rayOrg, D3DXVECTOR3 &rayDir, float decalSize)
{
	//Only supports skinned meshes for now
	if(pSkinInfo == NULL)
		return NULL;

	D3DXINTERSECTINFO hitInfo = GetFace(rayOrg, rayDir);

	//No face was hit
	if(hitInfo.FaceIndex == 0xffffffff)
		return NULL;

	//Generate adjacency lookup table
	DWORD* adj = new DWORD[OriginalMesh->GetNumFaces() * 3];
	OriginalMesh->GenerateAdjacency(0.001f, adj);

	//Get Vertex & index buffer of temp mesh
	SimpleDeclVertex *v = NULL;
	WORD *i = NULL;
	OriginalMesh->LockVertexBuffer(D3DLOCK_READONLY, (VOID**)&v);
	OriginalMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&i);

	//Calculate hit position on original mesh
	WORD i1 = i[hitInfo.FaceIndex * 3 + 0];
	WORD i2 = i[hitInfo.FaceIndex * 3 + 1];
	WORD i3 = i[hitInfo.FaceIndex * 3 + 2];
	D3DXVECTOR3 hitPos;
	D3DXVec3BaryCentric(&hitPos, 
		&v[i1].position, 
		&v[i2].position, 
		&v[i3].position, 
		hitInfo.U, 
		hitInfo.V);

	//Find adjacent faces within in range of hit location
	std::queue<WORD> openFaces;
	std::map<WORD, bool> decalFaces;

	//Add first face
	openFaces.push((WORD)hitInfo.FaceIndex);

	while(!openFaces.empty())
	{
		//Get first face
		WORD face = openFaces.front();
		openFaces.pop();

		//Get Triangle data for open face
		WORD i1 = i[face * 3 + 0];
		WORD i2 = i[face * 3 + 1];
		WORD i3 = i[face * 3 + 2];
		D3DXVECTOR3 &v1 = v[i1].position;
		D3DXVECTOR3 &v2 = v[i2].position;
		D3DXVECTOR3 &v3 = v[i3].position;

		float testSize = max(decalSize, 0.1f);

		//Should this face be added?
		if(D3DXVec3Length(&(hitPos - v1)) < testSize ||
			D3DXVec3Length(&(hitPos - v2)) < testSize ||
			D3DXVec3Length(&(hitPos - v3)) < testSize ||
			decalFaces.empty())
		{
			decalFaces[face] = true;

			//Add adjacent faces to open queue
			for(int a=0; a<3; a++)
			{
				DWORD adjFace = adj[face * 3 + a];

				if(adjFace != 0xffffffff)
				{
					//Check that it hasnt been added to decal faces
					if(decalFaces.count((WORD)adjFace) == 0)
						openFaces.push((WORD)adjFace);
				}
			}
		}
	}

	OriginalMesh->UnlockIndexBuffer();
	OriginalMesh->UnlockVertexBuffer();

	SAFE_DELETE_ARRAY(adj);

	//Create decal mesh
	ID3DXMesh* decalMesh = NULL;

	//No faces to create decal with
	if(decalFaces.empty())
		return NULL;

	//Create a new mesh from selected faces
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	MeshData.pMesh->GetDeclaration(decl);

	D3DXCreateMesh((int)decalFaces.size(), 
		(int)decalFaces.size() * 3, 
		D3DXMESH_MANAGED, 
		decl, 
		DXUTGetD3D9Device(), 
		&decalMesh);

	//Lock dest & src buffers
	DecalVertex* vDest = NULL;
	WORD* iDest = NULL;
	DecalVertex* vSrc = NULL;
	WORD* iSrc = NULL;
	decalMesh->LockVertexBuffer(0, (VOID**)&vDest);
	decalMesh->LockIndexBuffer(0, (VOID**)&iDest);
	MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (VOID**)&vSrc);
	MeshData.pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&iSrc);

	//Iterate through all faces in the decalFaces map
	std::map<WORD, bool>::iterator f;
	int index = 0;
	for(f=decalFaces.begin(); f!=decalFaces.end(); f++)
	{
		WORD faceIndex = (*f).first;

		//Copy vertex data
		vDest[index * 3 + 0] = vSrc[iSrc[faceIndex * 3 + 0]];
		vDest[index * 3 + 1] = vSrc[iSrc[faceIndex * 3 + 1]];
		vDest[index * 3 + 2] = vSrc[iSrc[faceIndex * 3 + 2]];

		//Create indices
		iDest[index * 3 + 0] = index * 3 + 0;
		iDest[index * 3 + 1] = index * 3 + 1;
		iDest[index * 3 + 2] = index * 3 + 2;

		index++;
	}

	//Unlock buffers
	decalMesh->UnlockIndexBuffer();
	decalMesh->UnlockVertexBuffer();
	MeshData.pMesh->UnlockIndexBuffer();
	MeshData.pMesh->UnlockIndexBuffer();

	//Calculate Decal UV coordinates
	CalculateDecalUV(decalMesh, hitPos, decalSize);

	return decalMesh;
}

ID3DXMesh* BoneMesh::GetHitMesh(D3DXVECTOR3 &rayOrg, D3DXVECTOR3 &rayDir)
{
	//Only supports skinned meshes for now
	if(pSkinInfo == NULL)
		return NULL;

	D3DXINTERSECTINFO hitInfo = GetFace(rayOrg, rayDir);

	//No face was hit
	if(hitInfo.FaceIndex == 0xffffffff)
		return NULL;

	//Get source Vertex & index buffer
	SimpleDeclVertex *v = NULL;
	WORD *i = NULL;
	OriginalMesh->LockVertexBuffer(D3DLOCK_READONLY, (VOID**)&v);
	OriginalMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&i);

	//Create decal mesh
	ID3DXMesh* decalMesh = NULL;

	//Create a new mesh from selected faces
	D3DXCreateMeshFVF(1, 
		3, 
		D3DXMESH_MANAGED, 
		SimpleDeclVertex::FVF, 
		DXUTGetD3D9Device(), 
		&decalMesh);

	SimpleDeclVertex *vDest = NULL;
	WORD *iDest = NULL;
	decalMesh->LockVertexBuffer(0, (VOID**)&vDest);
	decalMesh->LockIndexBuffer(0, (VOID**)&iDest);

	vDest[0] = v[i[hitInfo.FaceIndex * 3 + 0]];
	vDest[1] = v[i[hitInfo.FaceIndex * 3 + 1]];
	vDest[2] = v[i[hitInfo.FaceIndex * 3 + 2]];

	iDest[0] = 0;
	iDest[1] = 1;
	iDest[2] = 2;

	//unlock buffers
	decalMesh->UnlockIndexBuffer();
	decalMesh->UnlockVertexBuffer();
	OriginalMesh->UnlockIndexBuffer();
	OriginalMesh->UnlockVertexBuffer();

	return decalMesh;
}

void BoneMesh::AddDecal(D3DXVECTOR3 &rayOrg, D3DXVECTOR3 &rayDir, float decalSize)
{
	//Create decal mesh
	ID3DXMesh* decalMesh = CreateDecalMesh(rayOrg, rayDir, decalSize);
	if(decalMesh == NULL)
		return;

	//Add to decal vector
	m_decals.push_back(new CharacterDecal(decalMesh));
}

BoneMesh::~BoneMesh()
{
	for (int i = 0; i < m_decals.size(); ++i)
	{
		SAFE_DELETE(m_decals[i]);
	}
	
	m_decals.clear();

}

void BoneMesh::CalculateDecalUV(ID3DXMesh* decalMesh, D3DXVECTOR3 &hitPos, float decalSize)
{
	DecalVertex *v = NULL;
	decalMesh->LockVertexBuffer(0, (VOID**)&v);

	//Get Hit normal (first 3 vertices make up the hit triangle)
	DecalVertex &v1 = v[0];
	DecalVertex &v2 = v[1];
	DecalVertex &v3 = v[2];
	D3DXVECTOR3 faceNormal = v1.normal + 
		v2.normal + 
		v3.normal;
	D3DXVec3Normalize(&faceNormal, &faceNormal);

	//Calculate Right & Up vector
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &faceNormal, &up);
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Cross(&up, &faceNormal, &right);
	D3DXVec3Normalize(&up, &up);

	D3DXVECTOR3 decalCorner = (hitPos - right * decalSize - up * decalSize);

	D3DXVECTOR3 UCompare = -right * decalSize * 2.0f;
	D3DXVECTOR3 VCompare = -up * decalSize * 2.0f;

	//Loop through all vertices in the mesh and calculate their UV coordinates
	for(int i=0; i<(int)decalMesh->GetNumVertices(); i++)
	{
		D3DXVECTOR3 cornerToVertex = decalCorner - v[i].position;

		float U = D3DXVec3Dot(&cornerToVertex, &UCompare) / (decalSize / 4.0f);
		float V = D3DXVec3Dot(&cornerToVertex, &VCompare) / (decalSize / 4.0f);

		//Assign new UV coordinate to the vertex
		v[i].uv = D3DXVECTOR2(U, V);
	}

	decalMesh->UnlockVertexBuffer();
}

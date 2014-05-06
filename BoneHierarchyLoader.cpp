#include "DXUT.h"
#include "SDKmisc.h"
#include "BoneHierarchyLoader.h"
#include "skinnedMesh.h"
#include "Utils.h"

HRESULT BoneHierarchyLoaderSoft::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	frameCount++;
	Bone *newBone = new Bone;

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

HRESULT BoneHierarchyLoaderSoft::DestroyFrame(LPD3DXFRAME pFrameToFree) 
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

HRESULT BoneHierarchyLoaderSoft::CreateMeshContainer(LPCSTR Name,
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

	// Get Mesh Data
	boneMesh->OriginalMesh = pMeshData->pMesh;
	boneMesh->MeshData.pMesh = pMeshData->pMesh;
	boneMesh->MeshData.Type	= pMeshData->Type;

	pMeshData->pMesh->AddRef();

	for (size_t i = 0; i < NumMaterials; ++i)
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
			std::wstring conv = GetWC(textureFileName);

			WCHAR str[MAX_PATH];
			HRESULT hr;
			V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str() ) );

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

HRESULT BoneHierarchyLoaderSoft::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	BoneMesh* boneMesh = static_cast<BoneMesh*>(pMeshContainerBase);

	if(pMeshContainerBase != NULL)
	{	
		for (size_t i = 0; i < boneMesh->textures.size(); ++i)
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


HRESULT BoneHierarchyLoaderHAL::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	frameCount++;
	Bone *newBone = new Bone;

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

HRESULT BoneHierarchyLoaderHAL::DestroyFrame(LPD3DXFRAME pFrameToFree) 
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

HRESULT BoneHierarchyLoaderHAL::CreateMeshContainer(LPCSTR Name,
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

	// Get Mesh Data
	boneMesh->OriginalMesh = pMeshData->pMesh;
	boneMesh->MeshData.pMesh = pMeshData->pMesh;
	boneMesh->MeshData.Type	= pMeshData->Type;

	pMeshData->pMesh->AddRef();

	for (size_t i = 0; i < NumMaterials; ++i)
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
			std::wstring conv = GetWC(textureFileName);

			WCHAR str[MAX_PATH];
			HRESULT hr;
			V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str() ) );

			D3DXCreateTextureFromFile( DXUTGetD3D9Device(), str, &itexture);
		}

		boneMesh->textures.push_back((IDirect3DTexture9*)itexture);
	}

	if ( pSkinInfo )
	{
		boneMesh->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();	//Add reference so that the SkinInfo isnt deallocated

// 		//Clone mesh and store in boneMesh->MeshData.pMesh
// 		pMeshData->pMesh->CloneMeshFVF( D3DXMESH_MANAGED, pMeshData->pMesh->GetFVF(),
// 			DXUTGetD3D9Device(), &boneMesh->MeshData.pMesh);

		DWORD maxVertexInfluences = 0;
		DWORD numBoneComboEntries = 0;
		ID3DXBuffer* boneComboTable = 0;


		pSkinInfo->ConvertToIndexedBlendedMesh( pMeshData->pMesh,
												D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, 
												35,
												NULL,
												NULL,
												NULL,
												NULL,
												&maxVertexInfluences,
												&numBoneComboEntries,
												&boneComboTable,
												&boneMesh->MeshData.pMesh
												);

		// Bone Combination table not used
		if ( boneComboTable != NULL )
			boneComboTable->Release();

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

HRESULT BoneHierarchyLoaderHAL::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	BoneMesh* boneMesh = static_cast<BoneMesh*>(pMeshContainerBase);

	if(pMeshContainerBase != NULL)
	{	
		for (size_t i = 0; i < boneMesh->textures.size(); ++i)
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

STDMETHODIMP FaceHierarchyLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	D3DXFRAME* newBone = new D3DXFRAME();
	memset( newBone, 0, sizeof(D3DXFRAME));

	// Copy Name
	if( Name != NULL )
	{
		newBone->Name = new char[strlen(Name) + 1];
		strcpy( newBone->Name, Name);
	}

	D3DXMatrixIdentity(&newBone->TransformationMatrix);

	*ppNewFrame = newBone;

	return S_OK;
}



STDMETHODIMP FaceHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	if (pFrameToFree)
	{
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

STDMETHODIMP FaceHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase)
{
	ID3DXMesh* m = (ID3DXMesh*)pMeshContainerBase;
	SAFE_RELEASE(m);

	return S_OK;
}

STDMETHODIMP FaceHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	// Add Reference
	pMeshData->pMesh->AddRef();

	*ppNewMeshContainer = (D3DXMESHCONTAINER*)pMeshData->pMesh;

	return S_OK;
}

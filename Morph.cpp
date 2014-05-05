#include "DXUT.h"
#include "Morph.h"
#include "Utils.h"
#include "SDKmisc.h"


Morph::Morph(void)
{
	m_Target01 = NULL;
	m_Target02 = NULL;
	m_face = NULL;
	m_blend = 0.0f;
}


Morph::~Morph(void)
{
	SAFE_RELEASE(m_Target01);
	SAFE_RELEASE(m_Target02);
	SAFE_RELEASE(m_face);
}

void Morph::Init()
{
	// Load Targets
	std::wstring conv = GetWC("meshes\\face01.x");

	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str() );
	D3DXLoadMeshFromX(str, D3DXMESH_MANAGED, DXUTGetD3D9Device(), NULL, NULL, NULL, NULL, &m_Target01);

	conv = GetWC("meshes\\face02.x");
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str());
	D3DXLoadMeshFromX(str, D3DXMESH_MANAGED, DXUTGetD3D9Device(), NULL, NULL, NULL, NULL, &m_Target02);

	// Create the face mesh as a clone of one of the target mesh
	m_Target01->CloneMeshFVF( D3DXMESH_MANAGED, m_Target01->GetFVF(), DXUTGetD3D9Device(), &m_face);
}

void Morph::Update()
{
	BYTE* target01,* target02, *face;

	// Lock vertex buffers
	m_Target01->LockVertexBuffer( D3DLOCK_READONLY, (void**)&target01);
	m_Target02->LockVertexBuffer( D3DLOCK_READONLY, (void**)&target02);
	m_face->LockVertexBuffer( 0, (void**)&face);

	for (int i = 0; i < m_face->GetNumVertices(); ++i)
	{
		D3DXVECTOR3 t1 = *((D3DXVECTOR3*)target01);
		D3DXVECTOR3 t2 = *((D3DXVECTOR3*)target02);
		D3DXVECTOR3 *f = (D3DXVECTOR3 *)face;

		// Perform morph
		*f = t2 * m_blend + t1 * ( 1.0f - m_blend);

		// Move to next vertex
		target01 += m_Target01->GetNumBytesPerVertex();
		target02 += m_Target02->GetNumBytesPerVertex();
		face += m_face->GetNumBytesPerVertex();
	}

	m_face->UnlockVertexBuffer();
	m_Target02->UnlockVertexBuffer();
	m_Target01->UnlockVertexBuffer();
}

void Morph::Render(const char* tech)
{
	D3DXHANDLE techHandle = g_pEffect->GetTechniqueByName(tech);
	g_pEffect->SetTechnique(techHandle);

	g_pEffect->Begin(NULL, NULL);
	g_pEffect->BeginPass(0);

	m_face->DrawSubset(0);

	g_pEffect->EndPass();
	g_pEffect->End();
}


//Our morph vertex format
D3DVERTEXELEMENT9 morphVertexDecl[] = 
{
	//1st Stream: Base Mesh
	{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},	
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
	{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},	

	//2nd Stream: 1st Morph Target
	{1,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 1},
	{1, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   1},
	{1, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},

	//3rd Stream: 2st Morph Target
	{2,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 2},
	{2, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   2},
	{2, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},

	//4th Stream: 3st Morph Target
	{3,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 3},
	{3, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   3},
	{3, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},

	//5th Stream: 4st Morph Target
	{4,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 4},
	{4, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   4},
	{4, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},

	D3DDECL_END()
};

MultiMorph::MultiMorph()
{
	m_baseMesh = NULL;
	m_targets[0] = NULL;
	m_targets[1] = NULL;
	m_targets[2] = NULL;
	m_targets[3] = NULL;

	m_decl = NULL;

	m_oldWeights = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	m_newWeights = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	m_prc = 1.0f;
}

MultiMorph::~MultiMorph()
{
	SAFE_RELEASE(m_baseMesh);

	for (int i = 0; i < 4; ++i)
	{
		SAFE_RELEASE(m_targets[i]);
	}

	SAFE_RELEASE(m_decl);
}

void MultiMorph::Init()
{
	// Create Vertex Declaration
	DXUTGetD3D9Device()->CreateVertexDeclaration( morphVertexDecl, &m_decl);

	// Load Base Mesh
	LoadMesh("meshes\\baseMesh.x", &m_baseMesh);

	// Load Targets
	for (int i = 0; i <4 ; ++i)
	{
		std::string baseName = "meshes\\target0";
		baseName += ('1' + i);
		baseName += ".x";
		LoadMesh( baseName.c_str(), &(m_targets[i]));
	}
}

void MultiMorph::Update(float deltaTime)
{
	m_prc += deltaTime * 1.0f;

	if(m_prc > 1.0f)
	{
		m_prc = 0.0f;
		m_oldWeights = m_newWeights;	
		m_newWeights = D3DXVECTOR4(max(((rand()%1000) / 1000.0f) * 2.0f - 1.0f, 0.0f), 
			max(((rand()%1000) / 1000.0f) * 2.0f - 1.0f, 0.0f), 
			max(((rand()%1000) / 1000.0f) * 2.0f - 1.0f, 0.0f), 
			max(((rand()%1000) / 1000.0f) * 2.0f - 1.0f, 0.0f));
	}
}

void MultiMorph::Render(const char* tech)
{
	DXUTGetD3D9Device()->SetVertexDeclaration(m_decl);

	DWORD vSize = D3DXGetFVFVertexSize(m_baseMesh->GetFVF());

	// set base stream
	IDirect3DVertexBuffer9* baseMeshBuffer = NULL;
	IDirect3DIndexBuffer9* ib = NULL;
	
	m_baseMesh->GetVertexBuffer(&baseMeshBuffer);
	m_baseMesh->GetIndexBuffer(&ib);

	DXUTGetD3D9Device()->SetStreamSource( 0, baseMeshBuffer, 0, vSize);
	DXUTGetD3D9Device()->SetIndices(ib);

	// set target streams
	for (int i = 0; i < 4; ++i)
	{
		IDirect3DVertexBuffer9* targetMeshBuffer = NULL;
		m_targets[i]->GetVertexBuffer(&targetMeshBuffer);
		DXUTGetD3D9Device()->SetStreamSource( i + 1, targetMeshBuffer, 0, vSize);
		SAFE_RELEASE(targetMeshBuffer);
	}

	D3DXVECTOR4 weights = m_newWeights * m_prc + (1.0f - m_prc) * m_oldWeights;
	g_pEffect->SetVector("weights", &weights);

	D3DXHANDLE hTech = g_pEffect->GetTechniqueByName("MultiMorph");
	g_pEffect->SetTechnique(hTech);
	g_pEffect->Begin(NULL, NULL);
	g_pEffect->BeginPass(0);

	//Draw mesh
	DXUTGetD3D9Device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 
		m_baseMesh->GetNumVertices(), 0, 
		m_baseMesh->GetNumFaces());

	g_pEffect->EndPass();
	g_pEffect->End();

	//Restore
	DXUTGetD3D9Device()->SetVertexDeclaration(NULL);

	//Set Stream Sources to NULL
	for(int i=0;i<5;i++)
	{
		DXUTGetD3D9Device()->SetStreamSource(i, NULL, 0, 0);
	}

	SAFE_RELEASE(baseMeshBuffer);
	SAFE_RELEASE(ib);
}


D3DVERTEXELEMENT9 morphSkeletonVertexDecl[] = 
{
	//1st Stream: Human Skinned Mesh
	{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},	
	{0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
	{0, 16, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
	{0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
	{0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},	

	//2nd Stream: Werewolf Morph Target
	{1,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 1},
	{1, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   1},

	D3DDECL_END()
};

MorphMesh::MorphMesh() : SkinnedMesh()
{
	m_pDecl = NULL;
	m_animControl = NULL;
	m_pHumanTexture = NULL;
	m_pWolfTexture = NULL;
}

MorphMesh::~MorphMesh()
{
	SAFE_RELEASE( m_pDecl );
	SAFE_RELEASE( m_animControl );
	SAFE_RELEASE( m_pHumanTexture );
	SAFE_RELEASE( m_pWolfTexture );
}

void MorphMesh::Init()
{
	// create vertex declaration
	DXUTGetD3D9Device()->CreateVertexDeclaration( morphSkeletonVertexDecl, &m_pDecl);

	// Load Base Mesh
	Load(L"meshes\\werewolf.x");
	m_animControl = GetController();

	// Load Textures
	LoadTex( "meshes\\human.jpg", &m_pHumanTexture);
	LoadTex( "meshes\\werewolf.jpg", &m_pWolfTexture);
}

void MorphMesh::Update(float deltaTime)
{
	//Update Skinned Mesh
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	m_animControl->AdvanceTime(deltaTime, NULL);
	SetPose(world); 
}

void MorphMesh::Render()
{
	// set render param
	g_pEffect->SetTexture("texHuman", m_pHumanTexture);
	g_pEffect->SetTexture("texWolf", m_pWolfTexture);

	float shift = max(min(sin(GetTickCount() * 0.0005f) * 1.5f + 0.5f, 1.0f), 0.0f);
	g_pEffect->SetFloat("shapeShift", shift);

	// set vertex decl
	DXUTGetD3D9Device()->SetVertexDeclaration(m_pDecl);

	// set werewolf & human stream
	D3DXFRAME* wolfBone = D3DXFrameFind( m_pRootBone, "werewolf");
	if( wolfBone != NULL )
	{
		if(wolfBone->pMeshContainer != NULL)
		{
			ID3DXMesh* wolfmesh = wolfBone->pMeshContainer->MeshData.pMesh;
			DWORD vSize = D3DXGetFVFVertexSize(wolfmesh->GetFVF());
			IDirect3DVertexBuffer9* wolfMeshBuffer = NULL;
			wolfmesh->GetVertexBuffer(&wolfMeshBuffer);
			DXUTGetD3D9Device()->SetStreamSource(1, wolfMeshBuffer, 0, vSize);

			SAFE_RELEASE(wolfMeshBuffer);
		}
	}

	//Render Human / Werewolf
	D3DXFRAME* humanBone = D3DXFrameFind(m_pRootBone, "Bruce");
	RenderHuman((Bone*)humanBone);
}

void MorphMesh::RenderHuman(Bone *bone)
{
	if( bone == NULL )
		bone = (Bone*)m_pRootBone;


	if ( bone->pMeshContainer != NULL )
	{
		BoneMesh* boneMesh = (BoneMesh*)bone->pMeshContainer;

		if ( boneMesh->pSkinInfo != NULL )
		{
			// set up bone transform
			int numBones = boneMesh->pSkinInfo->GetNumBones();
			for (int i = 0; i < numBones; ++i)
			{
				D3DXMatrixMultiply( &boneMesh->currentBoneMatrices[i],
									&boneMesh->boneOffsetMatrices[i],
									boneMesh->boneMatrixPtrs[i] );
			}

			g_pEffect->SetMatrixArray("MatrixPalette", 
				boneMesh->currentBoneMatrices, 
				boneMesh->pSkinInfo->GetNumBones());

			// Get Size per vertex in bytes
			DWORD vSize = D3DXGetFVFVertexSize( boneMesh->MeshData.pMesh->GetFVF());

			// Set base stream (human)
			IDirect3DVertexBuffer9* baseMeshBuffer = NULL;
			boneMesh->MeshData.pMesh->GetVertexBuffer( & baseMeshBuffer);
			DXUTGetD3D9Device()->SetStreamSource( 0, baseMeshBuffer, 0, vSize);
			SAFE_RELEASE(baseMeshBuffer);

			// Set Index Buffer
			IDirect3DIndexBuffer9* ib = NULL;
			boneMesh->MeshData.pMesh->GetIndexBuffer(&ib);
			DXUTGetD3D9Device()->SetIndices( ib );
			SAFE_RELEASE(ib);

			//Start Shader
			D3DXHANDLE hTech = g_pEffect->GetTechniqueByName("MorphSkeleton");
			g_pEffect->SetTechnique(hTech);
			g_pEffect->Begin(NULL, NULL);
			g_pEffect->BeginPass(0);

			//Draw mesh
			DXUTGetD3D9Device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 
				boneMesh->MeshData.pMesh->GetNumVertices(), 0, 
				boneMesh->MeshData.pMesh->GetNumFaces());

			g_pEffect->EndPass();
			g_pEffect->End();
		}
	}

	if(bone->pFrameSibling != NULL)RenderHuman((Bone*)bone->pFrameSibling);
	if(bone->pFrameFirstChild != NULL)RenderHuman((Bone*)bone->pFrameFirstChild);
}

#include "DXUT.h"
#include "FaceFacory.h"
#include "BoneHierarchyLoader.h"
#include "Utils.h"
#include "Face.h"

struct FACEVERTEX
{
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_normal;
	D3DXVECTOR2 m_uv;

	static DWORD FVF;
};
DWORD FACEVERTEX::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX0;

FaceFacory::FaceFacory(const char* fileName)
{
	// initialize
	m_baseMesh = NULL;
	m_blinkMesh = NULL;
	m_eyeMesh = NULL;

	// Load face meshes
	FaceHierarchyLoader hierarchy;
	D3DXFRAME* root = NULL;
	LoadMeshHierarchy(fileName, &hierarchy, &root);

	// Extract face meshes
	ExtractMeshes(root);

	// Destroy temp hier
	hierarchy.DestroyFrame(root);

	//Load textures
	std::string texNames[8] = {"meshes\\face01.jpg", "meshes\\face02.jpg", "meshes\\face03.jpg", "meshes\\face04.jpg",
		"meshes\\face05.jpg", "meshes\\face06.jpg", "meshes\\face07.jpg", "meshes\\face08.jpg"};

	for(int i=0; i<8; i++)
	{
		IDirect3DTexture9* faceTexture = NULL;
		LoadTex( texNames[i].c_str(), &faceTexture);
		m_faceTexes.push_back(faceTexture);
	}
}


FaceFacory::~FaceFacory(void)
{
	SAFE_RELEASE(m_baseMesh);
	SAFE_RELEASE(m_blinkMesh);
	SAFE_RELEASE(m_eyeMesh);

	for (int i = 0; i < m_emotionMeshes.size(); ++i)
	{
		ID3DXMesh* mesh = m_emotionMeshes[i];
		SAFE_RELEASE(mesh);
	}

	for (int i = 0; i < m_speechMeshes.size(); ++i)
	{
		ID3DXMesh* mesh = m_speechMeshes[i];
		SAFE_RELEASE(mesh);
	}
	
	for (int i = 0; i < m_morphMeshes.size(); ++i)
	{
		ID3DXMesh* mesh = m_morphMeshes[i];
		SAFE_RELEASE(mesh);
	}

	for (int i = 0; i < m_faceTexes.size(); ++i)
	{
		IDirect3DTexture9* tex = m_faceTexes[i];
		SAFE_RELEASE(tex);
	}

}

void FaceFacory::ExtractMeshes(D3DXFRAME* frame)
{
	if (frame == NULL)
		return;

	if(frame->pMeshContainer)
	{
		if(frame->Name)
		{
			ID3DXMesh* mesh = (ID3DXMesh*)frame->pMeshContainer;

			if(!strcmp(frame->Name, "Base"))
			{
				m_baseMesh = mesh;
				mesh->AddRef();
			}
			else if(!strcmp(frame->Name, "Blink") )
			{
				m_blinkMesh = mesh;
				mesh->AddRef();
			}
			else if(!strcmp(frame->Name, "Emotion") )
			{
				m_emotionMeshes.push_back(mesh);
				mesh->AddRef();
			}
			else if(!strcmp(frame->Name, "Speech") )
			{
				m_speechMeshes.push_back(mesh);
				mesh->AddRef();
			}
			else
			{
				m_morphMeshes.push_back(mesh);
				mesh->AddRef();
			}
		}
	}

	ExtractMeshes(frame->pFrameFirstChild);
	ExtractMeshes(frame->pFrameSibling);
}

FaceModel* FaceFacory::GenerateRandomFace()
{
	// create random 0 - 1 morph weight for each morph target
	std::vector<float> morphWeights;

	for (int i = 0; i < (int)m_morphMeshes.size(); ++i)
	{
		float w = (rand() % 1000 ) / 1000.0f;
		morphWeights.push_back(w);
	}

	// next create a new empty face
	FaceModel* face = new FaceModel();

	// Then clone base, blink and all emotion , speech meshes
	face->m_baseMesh = CreateMorphTarget( m_baseMesh, morphWeights);
	face->m_binkMesh = CreateMorphTarget( m_blinkMesh, morphWeights);

	for (int i = 0; i < m_emotionMeshes.size(); ++i)
	{
		face->m_emotionMeshes.push_back(CreateMorphTarget(m_emotionMeshes[i], morphWeights));
	}

	for (int i = 0; i < m_speechMeshes.size(); ++i)
	{
		face->m_speechMeshes.push_back(CreateMorphTarget(m_speechMeshes[i], morphWeights));
	}

	// set a random face tex
	int index = rand() % (int)m_faceTexes.size();
	m_faceTexes[index]->AddRef();
	face->m_faceTex = m_faceTexes[index];

	return face;
}

ID3DXMesh* FaceFacory::CreateMorphTarget(ID3DXMesh* mesh, std::vector<float>& morphWeights)
{
	if( mesh == NULL || m_baseMesh == NULL )
		return NULL;

	// Clone mesh
	ID3DXMesh* newMesh = NULL;
	if(FAILED(mesh->CloneMeshFVF(D3DXMESH_MANAGED, mesh->GetFVF(), DXUTGetD3D9Device(), &newMesh)))
	{
		return NULL;
	}

	// copy base mesh data
	FACEVERTEX *vDest = NULL, *vSrc = NULL, *vMorph = NULL, *vBase = NULL;

	newMesh->LockVertexBuffer( 0, (void**)&vDest);
	m_baseMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vBase);

	mesh->LockVertexBuffer( D3DLOCK_READONLY, (void**)&vSrc);

	for (int i = 0; i < mesh->GetNumVertices(); ++i)
	{
		vDest[i].m_position = vSrc[i].m_position;
		vDest[i].m_normal = vSrc[i].m_normal;
		vDest[i].m_uv = vSrc[i].m_uv;
	}

	mesh->UnlockVertexBuffer();

	// morph base mesh using the provided weights
	for (int i = 0; i < m_morphMeshes.size(); ++i)
	{
		if( m_morphMeshes[i]->GetNumVertices() == mesh->GetNumVertices() )
		{
			m_morphMeshes[i]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vMorph);

			for (int j = 0; j < (int)mesh->GetNumVertices(); ++j)
			{
				vDest[j].m_position += ( vMorph[j].m_position - vBase[j].m_position ) * morphWeights[i];
				vDest[j].m_normal += ( vMorph[j].m_normal - vBase[j].m_normal ) * morphWeights[i];
			}

			m_morphMeshes[i]->UnlockVertexBuffer();
		}
	}

	m_baseMesh->UnlockVertexBuffer();
	newMesh->UnlockVertexBuffer();

	return newMesh;
}

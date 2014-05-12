#include "DXUT.h"
#include "BirdMesh.h"
#include "Utils.h"

BirdMesh::BirdMesh(void)
{
	m_mesh = NULL;
}

BirdMesh::BirdMesh(const char* fileName)
{
	m_mesh = NULL;
	Load(fileName);
}


BirdMesh::~BirdMesh(void)
{
	Release();
}

void BirdMesh::Release()
{
	SAFE_RELEASE(m_mesh);

	for (int i = 0; i < m_textures.size();++i)
		SAFE_RELEASE(m_textures[i]);

	m_textures.clear();
	m_materials.clear();
}

HRESULT BirdMesh::Load(const char* fileName)
{
	Release();

	// set m_white material
	m_white.Ambient = m_white.Specular = m_white.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	m_white.Emissive = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f);
	m_white.Power = 1.0f;

	// Load new mesh
	ID3DXBuffer* adjacencyBfr = NULL;
	ID3DXBuffer* materialBfr = NULL;
	DWORD materialNum = 0;

	LoadMesh( fileName, &m_mesh, &adjacencyBfr, &materialBfr, &materialNum);

	D3DXMATERIAL* mtrls = (D3DXMATERIAL*)materialBfr->GetBufferPointer();

	for (int i = 0; i < materialNum; ++i)
	{
		m_materials.push_back(mtrls[i].MatD3D);

		//Load textures for each material
		if(mtrls[i].pTextureFilename != NULL)
		{			
			IDirect3DTexture9 * newTexture = NULL;

			std::string path = "meshes\\";
			path += mtrls[i].pTextureFilename;
			LoadTex( path.c_str(), &newTexture);

			m_textures.push_back(newTexture);
		}
		else 
			m_textures.push_back(NULL);
	}

	m_mesh->OptimizeInplace( D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjacencyBfr->GetBufferPointer(), NULL, NULL, NULL);
	
	SAFE_RELEASE(materialBfr);
	SAFE_RELEASE(adjacencyBfr);

	return S_OK;
}

void BirdMesh::Render()
{
	int numMaterials = (int)m_materials.size();

	for (int i = 0; i < numMaterials; ++i)
	{
		// set material
		if(m_textures[i] != NULL)
			DXUTGetD3D9Device()->SetMaterial(&m_white);
		else
			DXUTGetD3D9Device()->SetMaterial(&m_materials[i]);

		g_pEffect->SetTexture( "g_MeshTexture", m_textures[i]);

		m_mesh->DrawSubset(i);
	}
}

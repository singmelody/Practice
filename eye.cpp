#include "DXUT.h"
#include "eye.h"
#include "Utils.h"

Eye::Eye()
{
}

Eye::~Eye()
{
	SAFE_RELEASE(sm_EyeMesh);

	for (int i = 0; i < sm_EyeTextures.size(); ++i)
	{
		SAFE_RELEASE(sm_EyeTextures[i]);
	}

	sm_EyeTextures.clear();

}

void Eye::Init(D3DXVECTOR3 position, int textureIndex)
{
	// Load Eye Resources
	if(sm_EyeMesh == NULL)
	{
		LoadMesh( "meshes\\eye.x", &sm_EyeMesh);
	}
	
	if(sm_EyeTextures.empty())
	{
		std::string eyeFiles[] = {
			"meshes\\eye01.jpg", "meshes\\eye02.jpg", 
			"meshes\\eye03.jpg", "meshes\\eye04.jpg", 
			"meshes\\eye05.jpg"};

		for(int i=0; i<5; i++)
		{
			IDirect3DTexture9* eyeTexture = NULL;
			LoadTex( eyeFiles[i].c_str(), &eyeTexture);
			sm_EyeTextures.push_back(eyeTexture);
		}
	}

	//Set Eye position
	m_position = position;

	m_textureIndex = textureIndex;
}

void Eye::Init(D3DXVECTOR3 position)
{
	// Load Eye Resources
	if(sm_EyeMesh == NULL)
	{
		LoadMesh( "meshes\\eye.x", &sm_EyeMesh);
	}

	if(sm_EyeTextures.empty())
	{
		std::string eyeFiles[] = {
			"meshes\\eye01.jpg", "meshes\\eye02.jpg", 
			"meshes\\eye03.jpg", "meshes\\eye04.jpg", 
			"meshes\\eye05.jpg"};

			for(int i=0; i<5; i++)
			{
				IDirect3DTexture9* eyeTexture = NULL;
				LoadTex( eyeFiles[i].c_str(), &eyeTexture);
				sm_EyeTextures.push_back(eyeTexture);
			}
	}

	//Set Eye position
	m_position = position;
}

void Eye::Render(D3DXMATRIX &headMatrix)
{
	D3DXMATRIX p;
	D3DXMatrixTranslation( &p, m_position.x, m_position.y, m_position.z);

	g_pEffect->SetMatrix("g_mWorld", &(m_rotation * p * headMatrix));
	g_pEffect->SetTexture( "g_MeshTexture", sm_EyeTextures[m_textureIndex]);
	g_pEffect->CommitChanges();

	sm_EyeMesh->DrawSubset(0);
}

void Eye::Render()
{
	D3DXMATRIX p;
	D3DXMatrixTranslation( &p, m_position.x, m_position.y, m_position.z);

	g_pEffect->SetMatrix("g_mWorld", &(m_rotation * p));
	g_pEffect->SetTexture( "g_MeshTexture", sm_EyeTextures[m_textureIndex]);
	g_pEffect->CommitChanges();

	sm_EyeMesh->DrawSubset(0);
}

void Eye::LookAt(D3DXVECTOR3 focus)
{
	float rotY = atan2(m_position.x - focus.x, m_position.z - focus.z) * 0.8f;
	float rotZ = atan2(m_position.y - focus.y, m_position.z - focus.z) * 0.5f;
	D3DXMatrixRotationYawPitchRoll(&m_rotation, rotY, rotZ, 0.0f);
}

ID3DXMesh* Eye::sm_EyeMesh = NULL;

std::vector<IDirect3DTexture9*> Eye::sm_EyeTextures;


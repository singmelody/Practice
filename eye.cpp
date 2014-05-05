#include "DXUT.h"
#include "eye.h"
#include "Utils.h"

Eye::Eye()
{
	m_EyeTexture = NULL;
	m_EyeMesh = NULL;
}

Eye::~Eye()
{
	SAFE_RELEASE(m_EyeTexture);
	SAFE_RELEASE(m_EyeMesh);
}

void Eye::Init(D3DXVECTOR3 position)
{
	// Load Eye Resources
	if(m_EyeMesh == NULL)
	{
		LoadMesh( "meshes\\eye.x", &m_EyeMesh);
	}
	
	if(m_EyeTexture == NULL)
	{
		LoadTex( "meshes\\eye.jpg", &m_EyeTexture);
	}

	//Set Eye position
	m_position = position;
}

void Eye::Render()
{
	D3DXMATRIX p;
	D3DXMatrixTranslation( &p, m_position.x, m_position.y, m_position.z);

	g_pEffect->SetMatrix("g_mWorld", &(m_rotation * p));
	g_pEffect->SetTexture( "g_MeshTexture", m_EyeTexture);
	g_pEffect->CommitChanges();

	m_EyeMesh->DrawSubset(0);
}

void Eye::LookAt(D3DXVECTOR3 focus)
{
	float rotY = atan2(m_position.x - focus.x, m_position.z - focus.z) * 0.8f;
	float rotZ = atan2(m_position.y - focus.y, m_position.z - focus.z) * 0.5f;
	D3DXMatrixRotationYawPitchRoll(&m_rotation, rotY, rotZ, 0.0f);
}


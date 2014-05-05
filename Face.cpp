#include "DXUT.h"
#include "Face.h"
#include "Utils.h"

Face::Face(void)
{
	m_faceMesh = NULL;
	m_faceTexture = NULL;
}

Face::~Face(void)
{
	SAFE_RELEASE(m_faceMesh);
	SAFE_RELEASE(m_faceTexture);
}

void Face::Init()
{
	// Load Mesh
	LoadMesh( "meshes\\face.x", &m_faceMesh);

	// Load Texture
	LoadTex( "meshes\\face.jpg", &m_faceTexture);

	//Init Eyes
	m_eyes[0].Init(D3DXVECTOR3(-0.037f, 0.04f, -0.057f));
	m_eyes[1].Init(D3DXVECTOR3( 0.037f, 0.04f, -0.057f));
}

void Face::Update(float deltaTime)
{

}

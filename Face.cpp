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
	POINT p;
	GetCursorPos(&p);

	float x = ( p.x - 320.0f) / 640.0f;
	float y = ( p.y - 240.0f) / 480.0f;

	// tell both eyes to look at point
	m_eyes[0].LookAt(D3DXVECTOR3(x,y,-1.0f));
	m_eyes[1].LookAt(D3DXVECTOR3(x,y,-1.0f));
}

void Face::Render(const char* tech)
{
	g_pEffect->SetTexture("g_MeshTexture", m_faceTexture);

	//Start Technique
	D3DXHANDLE hTech = g_pEffect->GetTechniqueByName(tech);
	g_pEffect->SetTechnique(hTech);
	g_pEffect->Begin(NULL, NULL);
	g_pEffect->BeginPass(0);

	//Render Face
	m_faceMesh->DrawSubset(0);

	//Render Eyes
	m_eyes[0].Render();
	m_eyes[1].Render();

	g_pEffect->EndPass();
	g_pEffect->End();
}

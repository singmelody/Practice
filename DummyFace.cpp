#include "DXUT.h"
#include "DummyFace.h"
#include "Utils.h"
//////////////////////////////////////////////////////////////////////////////////////////////////
//									DummyFace													//
//////////////////////////////////////////////////////////////////////////////////////////////////

DummyFace::DummyFace()
{
	m_pMesh = NULL;
	m_pTexture = NULL;

	LoadMesh( "meshes\\face01.x", &m_pMesh);
	LoadTex( "meshes\\face.jpg", &m_pTexture);
}

DummyFace::~DummyFace()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pTexture);
}

void DummyFace::Render()
{
	DXUTGetD3D9Device()->SetRenderState(D3DRS_LIGHTING, false);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	DXUTGetD3D9Device()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	DXUTGetD3D9Device()->SetTextureStageState(0, D3DTSS_ALPHAOP,  D3DTOP_SELECTARG1);

	D3DXMATRIX world;
	D3DXMatrixScaling(&world, 0.95f, 0.95f, 0.95f);
	//D3DXMatrixIdentity(&world);
	DXUTGetD3D9Device()->SetTransform(D3DTS_WORLD, &world);

	//Set Texture
	DXUTGetD3D9Device()->SetTexture(0, m_pTexture);

	//Render mesh
	m_pMesh->DrawSubset(0);
}

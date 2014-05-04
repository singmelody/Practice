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

#include "DXUT.h"
#include "CharacterDecal.h"
#include "Utils.h"

CharacterDecal::CharacterDecal(ID3DXMesh* pDecalMesh)
{
	m_pDecalMesh = pDecalMesh;
}

CharacterDecal::~CharacterDecal()
{
	SAFE_RELEASE(m_pDecalMesh);
}

void CharacterDecal::Render()
{
	D3DXHANDLE hTech = g_pEffect->GetTechniqueByName("Decal");
	g_pEffect->SetTechnique(hTech);
	g_pEffect->Begin(NULL, NULL);
	g_pEffect->BeginPass(0);

	m_pDecalMesh->DrawSubset(0);

	g_pEffect->EndPass();
	g_pEffect->End();
}
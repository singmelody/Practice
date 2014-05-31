#include "DXUT.h"
#include "CharacterDecal.h"
#include "Utils.h"

CharacterDecal::CharacterDecal(ID3DXMesh* pDecalMesh)
{
	m_pDecalMesh = pDecalMesh;

	if( sm_pDecalTexture == NULL )
	{
		LoadTex( "meshes\\decal.dds", &sm_pDecalTexture);
	}
}

CharacterDecal::~CharacterDecal()
{
	SAFE_RELEASE(m_pDecalMesh);
	SAFE_RELEASE(sm_pDecalTexture);
}

void CharacterDecal::Render()
{
	g_pEffect->SetTexture("g_DecalTexture", sm_pDecalTexture);

	D3DXHANDLE hTech = g_pEffect->GetTechniqueByName("Decal");
	g_pEffect->SetTechnique(hTech);
	g_pEffect->Begin(NULL, NULL);
	g_pEffect->BeginPass(0);

	m_pDecalMesh->DrawSubset(0);

	g_pEffect->EndPass();
	g_pEffect->End();
}

IDirect3DTexture9* CharacterDecal::sm_pDecalTexture = NULL;

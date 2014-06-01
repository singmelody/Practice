#include "DXUT.h"
#include "Hair.h"
#include "Utils.h"
#include "ControlHair.h"

Hair::Hair(void)
{
	//Test control hairs
	m_controlHairs.push_back(ControlHair::CreateTestHair(D3DXVECTOR3(-2.0f, 0.0f, -2.0f)));
	m_controlHairs.push_back(ControlHair::CreateTestHair(D3DXVECTOR3(2.0f, 0.0f, -2.0f)));
	m_controlHairs.push_back(ControlHair::CreateTestHair(D3DXVECTOR3(2.0f, 0.0f, 2.0f)));
	m_controlHairs.push_back(ControlHair::CreateTestHair(D3DXVECTOR3(-2.0f, 0.0f, 2.0f)));

	m_hairPatches.push_back(new HairPatch(m_controlHairs[0],
		m_controlHairs[1],
		m_controlHairs[2],
		m_controlHairs[3]));

	LoadTex( "meshes\\hair.png", &m_hairTex);
}


Hair::~Hair(void)
{
	for(int i=0; i<(int)m_controlHairs.size(); i++)
		SAFE_DELETE(m_controlHairs[i]);

	for(int i=0; i<(int)m_hairPatches.size(); i++)
		SAFE_DELETE(m_hairPatches[i]);

	SAFE_RELEASE(m_hairTex);
}

void Hair::Update(float deltaTime)
{
	//Update patches
	for(int i=0; i<(int)m_controlHairs.size(); i++)
		m_controlHairs[i]->Update(deltaTime);
}

void Hair::Render()
{
	//Render control hairs
	for(int i=0; i<(int)m_controlHairs.size(); i++)
		m_controlHairs[i]->Render();

	D3DXHANDLE hTech = g_pEffect->GetTechniqueByName("Hair");
	g_pEffect->SetTechnique(hTech);
	g_pEffect->SetTexture("g_MeshTexture", m_hairTex);
	g_pEffect->Begin(NULL, NULL);
	g_pEffect->BeginPass(0);

// 	if(KeyDown(VK_SPACE))
// 	{
// 		DXUTGetD3D9Device()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
// 		g_pEffect->SetTexture("g_MeshTexture", NULL);
// 		g_pEffect->CommitChanges();
// 	}
	//Render patches
	for(int i=0; i<(int)m_hairPatches.size(); i++)
		m_hairPatches[i]->Render();

	g_pEffect->EndPass();
	g_pEffect->End();

	DXUTGetD3D9Device()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

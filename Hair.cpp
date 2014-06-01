#include "DXUT.h"
#include "Hair.h"


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

	D3DXCreateTextureFromFile(g_pDevice, "resources/hair.png", &m_pHairTexture);
}


Hair::~Hair(void)
{
}

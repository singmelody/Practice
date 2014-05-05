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
	if(g_pEffect == NULL)
	{

	}
}


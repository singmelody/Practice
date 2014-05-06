#include "DXUT.h"
#include "FaceController.h"
#include "Utils.h"

FaceController::FaceController(D3DXVECTOR3 pos, FaceModel *pFace)
{
	m_faceModel = pFace;

	m_emotionIdx = 0;
	m_speechIndices[0] = 0;
	m_speechIndices[1] = 0;

	m_binkTime = -(1500+rand()%4000) / 1000.0f;
	m_emotionTime = 0.0f;
	m_emotionBlend = 0.0f;

	m_morphWeights = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);

	D3DXMatrixTranslation(&m_headMatrix, pos.x, pos.y, pos.z);

	//Init Eyes
	m_eyes[0].Init(D3DXVECTOR3(pos.x - 0.037f, pos.y + 0.04f, pos.z - 0.057f));
	m_eyes[1].Init(D3DXVECTOR3(pos.x + 0.037f, pos.y + 0.04f, pos.z - 0.057f));
}


FaceController::~FaceController(void)
{
}

void FaceController::Update(float deltaTime)
{
	if( m_faceModel == NULL)
		return;

	// Update morph weights
	float time = GetTickCount() * 0.002f;

	// Blink Eye-lids
	m_binkTime += deltaTime * 0.8f;
	if(m_binkTime >= 0.15f)
	{
		m_binkTime = -( 1500.0f + rand()% 4000) / 1000.0f;
	}

	float blink = 0.0f;
	if( m_binkTime > 0.0f )
		blink = sin(m_binkTime * D3DX_PI * 6.667f);

	// emotions
	m_emotionTime -= deltaTime;
	m_emotionBlend += ( m_emotionTime > 0.0f ? deltaTime : - deltaTime);

	// clamp emotion blend
	m_emotionBlend = min( 1.0f, m_emotionBlend);
	m_emotionBlend = max( 0.0f, m_emotionBlend);

	// select new emotion
	if( m_emotionTime <= 0.0f && m_emotionBlend <= 0.0f )
	{
		m_emotionBlend = 0.0f;
		m_emotionTime = ( 2000.0f + rand()% 6000) / 1000.0f;
		m_emotionIdx = rand() % (int)m_faceModel->m_emotionMeshes.size();
	}

	// Speech
	float speech1 = cos(time);
	if( speech1 < 0.0f)
	{
		m_speechIndices[0] = rand() % (int)m_faceModel->m_speechMeshes.size(); speech1 = 0.0f;
	}

	float speech2 = sin(time);
	if( speech1 < 0.0f )
	{
		m_speechIndices[1] = rand() % (int)m_faceModel->m_speechMeshes.size(); speech2 = 0.0f;
	}

	// Set Morph Weights
	m_morphWeights = D3DXVECTOR4(
			blink,
			m_emotionBlend,
			speech1,
			speech2
		);

	//Get look at point from cursor location
	POINT p;
	GetCursorPos(&p);

	float x = (p.x - 320) / 640.0f;
	float y = (p.y - 240) / 480.0f;

	//Tell both Eyes to look at this point
	m_eyes[0].LookAt(D3DXVECTOR3(x, y, -1.0f));
	m_eyes[1].LookAt(D3DXVECTOR3(x, y, -1.0f));
}

void FaceController::Render(const char* tech)
{
	if (m_faceModel)
	{
		m_faceModel->Render(this);

		//Render Eyes
		D3DXHANDLE hTech = g_pEffect->GetTechniqueByName(tech);
		g_pEffect->SetTechnique(hTech);
		g_pEffect->Begin(NULL, NULL);
		g_pEffect->BeginPass(0);

		m_eyes[0].Render();
		m_eyes[1].Render();

		g_pEffect->EndPass();
		g_pEffect->End();
	}
}

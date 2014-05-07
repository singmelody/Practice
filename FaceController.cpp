#include "DXUT.h"
#include "FaceController.h"
#include "Utils.h"
#include "WavDecoder.h"
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

	UpdateSpeech(deltaTime);

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


void FaceController::SpeakUpdate(float deltaTime)
{
	if( m_faceModel == NULL)
		return;

	UpdateSpeech(deltaTime);

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

	// Set Morph Weights
	m_morphWeights = D3DXVECTOR4(
		blink,
		m_emotionBlend,
		m_morphWeights.z,
		m_morphWeights.w
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

void FaceController::Speak(const std::vector<Viseme>& visemes)
{
	m_visemes.clear();

	m_visemes = visemes;

	// reset playback variable
	m_visemeIndex = 1;
	m_speechTime = 0.0f;
}

void FaceController::UpdateSpeech(float deltaTime)
{
	m_speechTime += deltaTime;

	if( m_visemeIndex > 0 && m_visemeIndex < m_visemes.size() )
	{
		// Get visemes to blend between
		Viseme& v1 = m_visemes[m_visemeIndex - 1];
		Viseme& v2 = m_visemes[m_visemeIndex];

		// Set speech meshes
		m_speechIndices[0] = v1.m_morphTarget % (int)m_faceModel->m_speechMeshes.size();
		m_speechIndices[1] = v2.m_morphTarget % (int)m_faceModel->m_speechMeshes.size();

		// set blend meshes
		float timeBetweenVisemes = v2.m_time - v1.m_time;
		float p = (m_speechTime - v1.m_time) / timeBetweenVisemes;
		m_morphWeights.z = (1.0f - p) * v1.m_blendWeight;
		m_morphWeights.w = p * v2.m_blendWeight;

		// update index
		if(m_speechTime >= v2.m_time)
			m_visemeIndex++;
	}
	else
	{
		m_morphWeights.z = 0.0f;
		m_morphWeights.w = 0.0f;
	}
}

void FaceController::SpeakWav(WavDecoder& wave)
{
	m_visemes.clear();

	// calculate which visemes to use from the wave file data
	float soundLength = wave.GetLength();
	float mapAmp = wave.GetMaxAmplitude() * 0.3f;

	for (float i = 0.0f; i < soundLength; i+=0.1f)
	{
		short amp = wave.GetAverageAmplitude( i, i+0.1f);
		float p = min( amp / mapAmp, 1.0f);

		if( p < 0.2f )
		{
			m_visemes.push_back(Viseme(0, 0.0f, i));
		}
		else if( p < 0.4f)
		{
			float prc = max( (p - 0.2f) /0.2f, 0.3f);
			m_visemes.push_back( Viseme(3, prc, i));
		}
		else if( p < 0.7f)
		{
			float prc = max( (p - 0.4f)/0.3f, 0.3f);
			m_visemes.push_back( Viseme( 1, prc, i));
		}
		else
		{
			float prc = max( (p-0.7f)/0.3f, 0.3f);
			m_visemes.push_back( Viseme( 4, prc, i));
		}
	}

	m_visemeIndex = 1;
	m_speechTime = 0.0f;
}

Viseme::Viseme()
{
	m_morphTarget = 0;
	m_blendWeight = 0.0f;
	m_time = 0.0f;
}

Viseme::Viseme(int target, float amount, float time)
{
	m_morphTarget = target;
	m_blendWeight = amount;
	m_time = time;
}

Viseme::~Viseme()
{

}
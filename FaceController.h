#pragma once

#include "Face.h"

class Viseme;
class WavDecoder;

class FaceController
{
	friend class face;
public:
	FaceController(D3DXVECTOR3 pos, FaceModel* pface);
	~FaceController(void);

	void Update(float deltaTime);
	void SpeakUpdate(float deltaTime);
	void Render(const char* tech);
	void RenderEyes();
	
	void Speak(const std::vector<Viseme>& visemes);
	void UpdateSpeech(float deltaTime);

	void SpeakWav(WavDecoder& wave);
public:
	FaceModel* m_faceModel;

	int m_emotionIdx;
	int m_speechIndices[2];

	float m_binkTime;
	float m_emotionTime,m_emotionBlend;

	D3DXVECTOR4 m_morphWeights;
	D3DXMATRIX	m_headMatrix;

	Eye m_eyes[2];

	// speech viseme array
	std::vector<Viseme> m_visemes;
	int m_visemeIndex;
	float m_speechTime;
};

class Viseme
{
public:
	Viseme();
	Viseme(int target, float amount, float time);
	~Viseme();

public:
	int m_morphTarget;
	float m_blendWeight;
	float m_time;
};
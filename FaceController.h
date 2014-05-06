#pragma once

#include "Face.h"

class FaceController
{
	friend class face;
public:
	FaceController(D3DXVECTOR3 pos, FaceModel* pface);
	~FaceController(void);

	void Update(float deltaTime);
	void Render(const char* tech);

public:
	FaceModel* m_faceModel;

	int m_emotionIdx;
	int m_speechIndices[2];

	float m_binkTime;
	float m_emotionTime,m_emotionBlend;

	D3DXVECTOR4 m_morphWeights;
	D3DXMATRIX	m_headMatrix;

	Eye m_eyes[2];
};


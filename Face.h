#pragma once

#include "Eye.h"

class Face
{
public:
	Face(void);
	~Face(void);

	void Init();
	void Update(float deltaTime);
	void Render(const char* tech);
	void RenderEye(const char* tech, D3DXVECTOR3 pos);

private:
	ID3DXMesh*			m_faceMesh;
	IDirect3DTexture9*	m_faceTexture;

	Eye		m_eyes[2];
};


#pragma once
#include "DXUT.h"

class Morph
{
public:
	Morph(void);
	~Morph(void);

	void Init();
	void Update();
	void Render(const char* tech);

	float GetBlend() { return m_blend; }
	void SetBlend(float blend) { m_blend = blend; }

private:
	ID3DXMesh* m_Target01,*m_Target02, *m_face;
	float m_blend;
};


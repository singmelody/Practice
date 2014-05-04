#pragma once
#include "DXUT.h"

class Morph
{
public:
	Morph(void);
	~Morph(void);

	void Init();
	void Update(float deltaTime);
	void Render(const char* tech);

private:
	ID3DXMesh* m_Target01,*m_Target02, *m_face;
	float m_blend;
};


#pragma once

#include <d3dx9.h>
#include "HairPatch.h"
#include <vector>

class ControlHair;
class BoundingSphere;

class Hair
{
public:
	Hair(void);
	Hair(bool b);
	~Hair(void);

	void LoadHair(const char* hairFile);

	void CreatePatch(int index1, int index2, int index3, int index4);

	int GetNumVertices();
	int GetNumFaces();

	void Update(float deltaTime);
	void Render();
	void RenderComplex(D3DXVECTOR3 &camPos);
public:
	std::vector<ControlHair*> m_controlHairs;
	std::vector<HairPatch*>	  m_hairPatches;
	IDirect3DTexture9*		  m_hairTex;
	std::vector<BoundingSphere> m_headSpheres;
};


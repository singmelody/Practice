#pragma once

#include <d3dx9.h>
#include "HairPatch.h"
#include <vector>

class ControlHair;

class Hair
{
public:
	Hair(void);
	~Hair(void);

	void Update(float deltaTime);
	void Render();
public:
	std::vector<ControlHair*> m_controlHairs;
	std::vector<HairPatch*>	  m_hairPatches;
	IDirect3DTexture9*		  m_hairTex;
};


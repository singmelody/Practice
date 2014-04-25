#pragma once

#include <DXUT.h>
#include <d3dx9.h>

class Animation
{
public:
	Animation();
	~Animation();

	void Update(float time);

	void Draw();
private:
	ID3DXKeyframedAnimationSet* m_animationSet;
	ID3DXCompressedAnimationSet* m_compressedAnimationSet;
	float m_time;
};
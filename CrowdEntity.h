#pragma once

#include "skinnedMesh.h"
#include "Crowd.h"

class CrowdEntity
{
	friend class Crowd;
public:
	CrowdEntity(Crowd *pCrowd);
	~CrowdEntity();
	void Update(float deltaTime);
	void Render();
	D3DXVECTOR3 GetRandomLocation();

private:
	static SkinnedMesh* sm_pSoldierMesh;

	Crowd* m_pCrowd;
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_velocity;
	D3DXVECTOR3 m_goal;

	ID3DXAnimationController* m_pAnimController;
};


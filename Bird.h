#pragma once

#include "Flock.h"
#include "BirdMesh.h"

class Bird
{
	friend class Flock;
public:
	Bird(Flock* p);
	~Bird(void);

	void Update(float deltaTime);
	void Render(bool shadow);

private:
	static BirdMesh* sm_birdMesh;

	Flock* m_pFlock;
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_velocity;
};


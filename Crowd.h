#pragma once

#include <vector>

class CrowdEntity;

class Crowd
{
public:
	Crowd(int numEntities);
	~Crowd(void);

	void Update(float deltaTime);
	void Render();
	void GetNeighbors(CrowdEntity* pEntity, float radius, std::vector<CrowdEntity*> &neighbors);

private:
	std::vector<CrowdEntity*> m_entities;
};


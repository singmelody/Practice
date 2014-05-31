#pragma once

#include <vector>

class CrowdEntity;
class BirdMesh;

class Obstacle
{
public:
	Obstacle(D3DXVECTOR3 pos, float radius);
	~Obstacle();
	D3DXVECTOR3 GetForce(CrowdEntity* pEntity);
	void Render();

public:
	static ID3DXMesh* sm_cylinder; 
	D3DXVECTOR3 m_position;
	float m_radius;
};

class Crowd
{
	friend CrowdEntity;
public:
	Crowd(int numEntities);
	~Crowd(void);

	void Update(float deltaTime);
	void Render();
	void GetNeighbors(CrowdEntity* pEntity, float radius, std::vector<CrowdEntity*> &neighbors);
	void SetEntityGroundPos(D3DXVECTOR3 &pos);

private:
	std::vector<CrowdEntity*> m_entities;
	std::vector<Obstacle*> m_obstacles;
	BirdMesh* m_pFloor;

};


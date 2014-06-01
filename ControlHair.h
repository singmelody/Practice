#pragma once

#include <d3dx9.h>
#include <vector>
#include "BoundingSphere.h"

using namespace std;

struct HairLineVertex{
	HairLineVertex(D3DXVECTOR3 pos, D3DCOLOR col){position = pos; color = col;}
	D3DXVECTOR3 position;
	D3DCOLOR color;
};

class ControlHair
{
public:
	ControlHair();

	static ControlHair* CreateTestHair(D3DXVECTOR3 startPos);

	float GetSegmentPercent(float prc);
	pair<int, int> GetBlendIndices(float prc);	
	D3DXVECTOR3 GetBlendedPoint(float prc);
	
	void Update(float deltaTime);
	void Render();	

	void AddPoint(D3DXVECTOR3 point);
	void UpdateSimulation(float deltaTime, vector<BoundingSphere> &headSpheres);

public:
	std::vector<D3DXVECTOR3> m_points;
	std::vector<D3DXVECTOR3> m_destPoints;

	std::vector<D3DXVECTOR3> m_originalPoints;
	std::vector<D3DXVECTOR3> m_velocities;
};


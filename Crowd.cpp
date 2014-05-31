#include "DXUT.h"
#include "Crowd.h"
#include "CrowdEntity.h"
#include "BirdMesh.h"
#include "Utils.h"

Crowd::Crowd(int numBoids)
{
	for(int i=0; i<numBoids; i++)
	{
		m_entities.push_back(new CrowdEntity(this));
	}

	m_obstacles.push_back(new Obstacle(D3DXVECTOR3(-5.0f, 0.0f, -3.0f), 1.0f));
	m_obstacles.push_back(new Obstacle(D3DXVECTOR3(5.0f, 0.0f, -3.0f), 0.5f));
	m_obstacles.push_back(new Obstacle(D3DXVECTOR3(-5.0f, 0.0f, 3.0f), 0.5f));
	m_obstacles.push_back(new Obstacle(D3DXVECTOR3(5.0f, 0.0f, 3.0f), 1.0f));
	m_obstacles.push_back(new Obstacle(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.75f));
	m_obstacles.push_back(new Obstacle(D3DXVECTOR3(-7.0f, 0.0f, 0.0f), 0.75f));
	m_obstacles.push_back(new Obstacle(D3DXVECTOR3(7.0f, 0.0f, 0.0f), 0.75f));
	m_obstacles.push_back(new Obstacle(D3DXVECTOR3(0.0f, 0.0f, -7.0f), 0.75f));
	m_obstacles.push_back(new Obstacle(D3DXVECTOR3(0.0f, 0.0f, 7.0f), 0.75f));

	m_pFloor = new BirdMesh("meshes\\floor.x");
}

Crowd::~Crowd()
{
	SAFE_DELETE(m_pFloor);

	for(int i=0; i<(int)m_entities.size(); i++)
		SAFE_DELETE(m_entities[i]);

	for (int i = 0; i < (int)m_obstacles.size(); ++i)
		SAFE_DELETE(m_obstacles[i]);

	m_entities.clear();
	m_obstacles.clear();
}

void Crowd::Update(float deltaTime)
{
	for(int i=0; i<(int)m_entities.size(); i++)
	{
		m_entities[i]->Update(deltaTime);
	}
}

void Crowd::Render()
{
	//Render floor
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	g_pEffect->SetMatrix("g_mWorld", &world);

	//Render floor
	m_pFloor->Render();

	//Render obstacles
	for(int i=0; i<(int)m_obstacles.size(); i++)
		m_obstacles[i]->Render();

	for(int i=0; i<(int)m_entities.size(); i++)
		m_entities[i]->Render();
}

void Crowd::GetNeighbors(CrowdEntity* pEntity, float radius, std::vector<CrowdEntity*> &neighbors)
{
	for(int i=0; i<(int)m_entities.size(); i++)
	{
		if(m_entities[i] != pEntity)
		{
			if(D3DXVec3Length(&(pEntity->m_position - m_entities[i]->m_position)) < radius)
			{
				neighbors.push_back(m_entities[i]);
			}
		}
	}
}

void Crowd::SetEntityGroundPos(D3DXVECTOR3 &pos)
{
	// create the test ray
	D3DXVECTOR3 org = pos + D3DXVECTOR3( 0.0f, 10.0f, 0.0f);
	D3DXVECTOR3 dir = D3DXVECTOR3( 0.0f, -1.0f, 0.0f);

	BOOL Hit;
	DWORD FaceIndex;
	float u,v,dist;

	D3DXIntersect( m_pFloor->GetMesh(), &org, &dir, &Hit, &FaceIndex, &u, &v, &dist, NULL, NULL);

	if(Hit)
	{
		pos.y = org.y - dist;
	}
}

Obstacle::Obstacle(D3DXVECTOR3 pos, float radius)
{
	m_position = pos;
	m_radius = radius;

	if(sm_cylinder == NULL)
	{
		D3DXCreateCylinder(DXUTGetD3D9Device(),
			1.0f,
			1.0f,
			6.0f,
			20,
			1,
			&sm_cylinder,
			NULL);
	}
}

Obstacle::~Obstacle()
{
	SAFE_RELEASE(sm_cylinder);
}

void Obstacle::Render()
{
	D3DXMATRIX world, sca, rot, pos;

	D3DXMatrixScaling(&sca, m_radius, m_radius, 1.0f);
	D3DXMatrixRotationX(&rot, D3DX_PI * 0.5f);
	D3DXMatrixTranslation(&pos, m_position.x, m_position.y, m_position.z);

	world = sca * rot * pos;

	g_pEffect->SetMatrix("g_mWorld", &world);
	g_pEffect->CommitChanges();

	sm_cylinder->DrawSubset(0);
}

D3DXVECTOR3 Obstacle::GetForce(CrowdEntity* pEntity)
{
	D3DXVECTOR3 vToEntity = m_position - pEntity->GetPos();
	float dist2Entity = D3DXVec3Length(&vToEntity);

	//Affected by this obstacle?
	if(dist2Entity < m_radius * 3.0f)
	{
		D3DXVec3Normalize(&vToEntity, &vToEntity);
		float force = 1.0f - (dist2Entity / m_radius * 3.0f);
		return vToEntity * force * 3.0f;
	}

	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


ID3DXMesh* Obstacle::sm_cylinder = NULL;

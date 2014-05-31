#include "DXUT.h"
#include "CrowdEntity.h"
#include "Utils.h"

CrowdEntity::CrowdEntity(Crowd *pCrowd)
{
	if(sm_pSoldierMesh == NULL)
	{
		//Load soldier mesh
		sm_pSoldierMesh = new SkinnedMesh();
		sm_pSoldierMesh->Load(L"meshes\\soldier.X");
	}

	m_pCrowd = pCrowd;

	m_position = GetRandomLocation();
	m_goal = GetRandomLocation();

	D3DXVec3Normalize(&m_velocity, &m_position);

	m_pAnimController = sm_pSoldierMesh->GetController();

	ID3DXAnimationSet *anim = NULL;
	m_pAnimController->GetAnimationSetByName("Walk", &anim);
	m_pAnimController->SetTrackAnimationSet(0, anim);
	SAFE_RELEASE(anim);
}

CrowdEntity::~CrowdEntity()
{
	SAFE_DELETE(sm_pSoldierMesh);
	SAFE_RELEASE(m_pAnimController);
}

D3DXVECTOR3 CrowdEntity::GetRandomLocation()
{
	return D3DXVECTOR3(((rand()%1000) / 1000.0f) * 20.0f - 10.0f,
		0.0f, ((rand()%1000) / 1000.0f) * 20.0f - 10.0f);
}

void CrowdEntity::Update(float deltaTime)
{
	const float ENTITY_INFLUENCE_RADIUS = 3.0f;
	const float NEIGHBOR_REPULSION = 5.0f;
	const float ENTITY_SPEED = 2.0f;
	const float ENTITY_SIZE = 1.0f;

	//Force towards goal
	D3DXVECTOR3 forceToGoal = m_goal - m_position;

	//Has goal been reached?
	if(D3DXVec3Length(&forceToGoal) < ENTITY_INFLUENCE_RADIUS)
	{
		//Pick a new random goal & pos
		m_position = GetRandomLocation();
		m_goal = GetRandomLocation();
	}
	D3DXVec3Normalize(&forceToGoal, &forceToGoal);

	std::vector<CrowdEntity*> neighbors;
	m_pCrowd->GetNeighbors(this, ENTITY_INFLUENCE_RADIUS, neighbors);

	//Avoid bumping into close neighbors
	D3DXVECTOR3 forceAvoidNeighbors(0.0f, 0.0f, 0.0f);
	for(int i=0; i<(int)neighbors.size(); i++)
	{
		D3DXVECTOR3 toNeighbor = neighbors[i]->m_position - m_position;
		float distToNeighbor = D3DXVec3Length(&toNeighbor);

		toNeighbor.y = 0.0f;
		float force = 1.0f - (distToNeighbor / ENTITY_INFLUENCE_RADIUS);
		forceAvoidNeighbors += -toNeighbor * NEIGHBOR_REPULSION * force;

		//Force move intersecting entities
		if(distToNeighbor < ENTITY_SIZE)
		{
			D3DXVECTOR3 center = (m_position + neighbors[i]->m_position) * 0.5f;
			D3DXVECTOR3 dir = center - m_position;
			D3DXVec3Normalize(&dir, &dir);

			//Force move both entities
			m_position = center - dir * ENTITY_SIZE * 0.5f;
			neighbors[i]->m_position = center + dir * ENTITY_SIZE * 0.5f;
		}
	}

	//Obstacle forces
	D3DXVECTOR3 forceObstacles(0.0f, 0.0f, 0.0f);
	for(int i=0; i<(int)m_pCrowd->m_obstacles.size(); i++)
	{
		forceObstacles += m_pCrowd->m_obstacles[i]->GetForce(this);
	}

	//Sum up forces
	D3DXVECTOR3 acc = forceToGoal + forceAvoidNeighbors + forceObstacles;
	D3DXVec3Normalize(&acc, &acc);

	//Update velocity & position
	m_velocity += acc * deltaTime;
	D3DXVec3Normalize(&m_velocity, &m_velocity);
	m_position += m_velocity * ENTITY_SPEED * deltaTime;

	//Snap to floor
	m_pCrowd->SetEntityGroundPos(m_position);

	//Update animation
	m_pAnimController->AdvanceTime(deltaTime, NULL);
}

void CrowdEntity::Render()
{
	D3DXMATRIX world, pos, rot;

	//Position
	D3DXMatrixTranslation(&pos, m_position.x, m_position.y, m_position.z);

	//Orientation
	float angle = atan2(m_velocity.z, -m_velocity.x) + D3DX_PI * 0.5f;
	D3DXMatrixRotationY(&rot, angle);

	//Set pose and render
	world = rot * pos;
	sm_pSoldierMesh->SetPose(world);
	sm_pSoldierMesh->RenderHAL(NULL, "SkinHAL", "RenderSceneWithTexture1Light");
}

SkinnedMesh* CrowdEntity::sm_pSoldierMesh = NULL;

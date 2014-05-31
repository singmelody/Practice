#include "DXUT.h"
#include "Bird.h"
#include "Utils.h"

Bird::Bird(Flock* p)
{
	if(sm_birdMesh == NULL)
	{
		//Load boid mesh
		sm_birdMesh = new BirdMesh("meshes\\boid.X");	
	}

	m_pFlock = p;

	m_position = D3DXVECTOR3(((rand()%1000) / 1000.0f) * 20.0f - 10.0f,
		((rand()%1000) / 1000.0f) * 20.0f,
		((rand()%1000) / 1000.0f) * 20.0f - 10.0f);

	D3DXVec3Normalize(&m_velocity, &m_position);
}


Bird::~Bird(void)
{
	SAFE_DELETE(sm_birdMesh);
}

void Bird::Update(float deltaTime)
{
	//Tweakable values
	const float NEIGHBORHOOD_SIZE = 2.5f;
	const float SEPARATION_LIMIT = 2.5f;
	const float SEPARATION_FORCE = 15.0f;
	const float BOID_SPEED = 3.0f;

	// Get neighbor birds
	std::vector<Bird*> neighbors;
	m_pFlock->GetNeighbors( this, NEIGHBORHOOD_SIZE, neighbors);

	//Forces
	D3DXVECTOR3 acceleration(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 separationForce(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 alignmentForce(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 cohesionForce(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 toPointForce(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 floorForce(0.0f, 0.0f, 0.0f);

	if(!neighbors.empty())
	{
		D3DXVECTOR3 center ( 0.0f, 0.0f, 0.0f);

		for (int i = 0; i < neighbors.size(); ++i)
			center += neighbors[i]->m_position;

		center /= (float)neighbors.size();

		// RULE 1 : separation
		for (int i = 0; i < neighbors.size(); ++i)
		{
			D3DXVECTOR3 offsetNeighbor = neighbors[i]->m_position - m_position;
			float dist2Neighbor = D3DXVec3Length(&offsetNeighbor);

			if( dist2Neighbor < SEPARATION_LIMIT )
			{
				// too close to neighbor
				float force =  1.0f - ( dist2Neighbor / SEPARATION_LIMIT );
				separationForce -= offsetNeighbor * SEPARATION_FORCE * force;
			}
		}

		// RULE 2 : Alignment
		for (int i = 0; i < neighbors.size(); ++i)
			alignmentForce += neighbors[i]->m_velocity;

		alignmentForce /= (float)neighbors.size();

		// RULE3 : Cohesion
		float dist2Center = D3DXVec3Length(&(center - m_position)) + 0.01f;
		cohesionForce = (center - m_position) / dist2Center;
	}

	// RULE 4 : Steer 2 point
	toPointForce = D3DXVECTOR3( 0.0f, 15.0f, 0.0f) - m_position;
	D3DXVec3Normalize( &toPointForce, &toPointForce);
	toPointForce *= 0.5f;

	// RULE 5 : Dont't crash !
	if( m_position.y < 3.0f )
		floorForce.y += (3.0f - m_position.y) * 100.0f;

	//Sum up forces
	acceleration = separationForce + 
		alignmentForce + 
		cohesionForce + 
		toPointForce + 
		floorForce;		

	//Update velocity & position
	D3DXVec3Normalize(&acceleration, &acceleration);
	m_velocity += acceleration * deltaTime * 3.0f;
	D3DXVec3Normalize(&m_velocity, &m_velocity);
	m_position += m_velocity * BOID_SPEED * deltaTime;

	//Cap Y position
	m_position.y = max(m_position.y, 1.0f);
}

void Bird::Render(bool shadow = false)
{
	D3DXMATRIX world, pos, rot;

	//Position
	D3DXMatrixTranslation(&pos, m_position.x, m_position.y, m_position.z);

	//Orientation
	D3DXMATRIX rotY, rotXZ;
	float angleY = atan2(m_velocity.x, m_velocity.z);
	float angleXZ = atan2(m_velocity.y, cos(angleY) * m_velocity.z);

	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &m_velocity, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	D3DXMatrixRotationY(&rotY, angleY);
	D3DXMatrixRotationAxis(&rotXZ, &right, angleXZ);

	rot = rotY * rotXZ;

	world = rot * pos;

	//Render boid
	g_pEffect->SetMatrix("g_mWorld", &world);
	g_pEffect->CommitChanges();
	sm_birdMesh->Render();
}

BirdMesh* Bird::sm_birdMesh = NULL;

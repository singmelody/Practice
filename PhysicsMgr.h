#pragma once
#pragma warning (disable:4239)
#include <DXUT.h>
#include "OBB.h"
#include <vector>
#include <btBulletDynamicsCommon.h>
#include "Utils.h"
#include "RagDoll.h"

//Bullet-to-DX Helper Functions
D3DXVECTOR3 BT2DX_VECTOR3(const btVector3 &v);
D3DXQUATERNION BT2DX_QUATERNION(const btQuaternion &q);
D3DXMATRIX BT2DX_MATRIX(const btMotionState &ms);
D3DXMATRIX BT2DX_MATRIX(const btTransform &ms);


class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	void Init();
	void Release();
	void Update(float deltaTime);
	//void Render(const char* tech,bool showOBB);
	void Render();

	void Reset();
 	void ResetBase();
// 	void ResetBox();
// 	void ResetJoint(PHYS_CONTRAINTS c);
// 	void ResetRagDoll();

	OBB* CreateOBB(D3DXVECTOR3 pos, D3DXVECTOR3 size, bool dynamic=true);

	D3DXVECTOR3 RandomVector(D3DXVECTOR3 &min, D3DXVECTOR3 &max);
	float RandomFloat();

	btDynamicsWorld* GetWorld(){return m_dynamicsWorld;}

	std::vector<RagDoll*>	m_ragdolls;
private:
	std::vector<OBB*>		m_boxes;
	btDynamicsWorld*		m_dynamicsWorld;
	btDefaultCollisionConfiguration* m_config;
	btRigidBody*			m_floor;
	btMotionState*			m_floorState;
	btCollisionShape*		m_floorShape;

	float					m_time;


	btBroadphaseInterface* m_broadphase;
	btCollisionDispatcher* m_dispatcher;
	btConstraintSolver* m_solver;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
};
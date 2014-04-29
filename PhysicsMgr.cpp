#include <DXUT.h>
#include "PhysicsMgr.h"

// Bullet2Dx Helper Functions
D3DXVECTOR3 BT2DX_VECTOR3(const btVector3& v)
{
	return D3DXVECTOR3(v.x(), v.y(), v.z() );
}

D3DXQUATERNION BT2DX_QUATERNION(const btQuaternion& v)
{
	return D3DXQUATERNION(v.x(), v.y(), v.z(), v.w());
}

D3DXMATRIX BT2DX_MATRIX(const btMotionState &ms)
{
	btTransform world;
	ms.getWorldTransform(world);
	return BT2DX_MATRIX(world);
}

D3DXMATRIX BT2DX_MATRIX(const btTransform& ms)
{
	btQuaternion q = ms.getRotation();
	btVector3 p = ms.getOrigin();

	D3DXMATRIX pos, rot, world;
	D3DXMatrixTranslation(&pos, p.x(), p.y(), p.z());
	D3DXMatrixRotationQuaternion(&rot, &BT2DX_QUATERNION(q));
	D3DXMatrixMultiply(&world, &rot, &pos);

	return world;
}

void PhysicsManager::Init()
{
	srand(GetTickCount());

	// New default Collision configuration
	m_config = new btDefaultCollisionConfiguration();

	// New default Constraint solver
	btConstraintSolver* sl;
	sl = new btSequentialImpulseConstraintSolver();

	//New axis sweep broadphase
	btVector3 worldAabbMin(-1000,-1000,-1000);
	btVector3 worldAabbMax(1000,1000,1000);

	const int maxProxies = 32766;
	btBroadphaseInterface *bp;
	bp = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);

	// new dispatcher
	btCollisionDispatcher* dp;
	dp = new btCollisionDispatcher(m_config);

	// finally create a dynamics 
	m_dynamicsWorld = new btDiscreteDynamicsWorld( dp, bp, sl, m_config);

	// Create floor plane
 	m_floor = new btRigidBody( 0.0f, new btDefaultMotionState(), new btStaticPlaneShape(btVector3(0,1,0), -0.5f));
 	m_dynamicsWorld->addRigidBody(m_floor);

	Reset();
}

void PhysicsManager::Release()
{
	for (int i = 0; i < m_boxes.size(); ++i)
	{
		m_boxes[i]->Release();
		SAFE_DELETE(m_boxes[i]);
	}

	m_boxes.clear();

	btCollisionObjectArray& array =  m_dynamicsWorld->getCollisionObjectArray();
	int Nums = m_dynamicsWorld->getNumCollisionObjects();
	for (int i = Nums - 1; i >= 0; --i)
	{
		btCollisionObject* obj = array[i];
		btRigidBody* b = btRigidBody::upcast(obj);

		btMotionState *state = b->getMotionState();
		SAFE_DELETE(state);

		btCollisionShape* shape = b->getCollisionShape();
		SAFE_DELETE(shape);

		m_dynamicsWorld->removeCollisionObject(obj);
		SAFE_DELETE(obj);
	}
	

	btDispatcher* dp = m_dynamicsWorld->getDispatcher();
	SAFE_DELETE(dp);

	btBroadphaseInterface* bp = m_dynamicsWorld->getBroadphase();
	SAFE_DELETE(bp);

	btConstraintSolver* sl = m_dynamicsWorld->getConstraintSolver();
	SAFE_DELETE(sl);

	SAFE_DELETE(m_config);

	SAFE_DELETE(m_dynamicsWorld);
}

void PhysicsManager::Reset()
{
	// Remove old boxes
	for (int i = 0; i  < m_boxes.size(); ++i)
	{
		m_dynamicsWorld->removeRigidBody(m_boxes[i]->m_Body);
		SAFE_DELETE(m_boxes[i]);
	}

	m_boxes.clear();

	// Create new ones...
	for (int i = 0; i < 100; ++i)
	{
		m_boxes.push_back(CreateOBB(RandomVector(D3DXVECTOR3(-10.0f, 0.0f, -10.0f), D3DXVECTOR3(10.0f, 50.0f, 10.0f)), 
			RandomVector(D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DXVECTOR3(3.0f, 3.0f, 3.0f))));
	}

}

D3DXVECTOR3 PhysicsManager::RandomVector(D3DXVECTOR3 &min, D3DXVECTOR3 &max)
{
	return D3DXVECTOR3(min.x + (max.x - min.x) * RandomFloat(),
		min.y + (max.y - min.y) * RandomFloat(),
		min.z + (max.z - min.z) * RandomFloat());
}

float PhysicsManager::RandomFloat()
{
	return (rand()%1000) / 1000.0f;
}

OBB* PhysicsManager::CreateOBB(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	OBB* obb = new OBB( pos, size);
	m_dynamicsWorld->addRigidBody(obb->m_Body);
	return obb;
}

PhysicsManager::PhysicsManager()
{

}

PhysicsManager::~PhysicsManager()
{

}

void PhysicsManager::Update(float deltaTime)
{
	if(m_dynamicsWorld != NULL)
	{
		m_dynamicsWorld->stepSimulation(deltaTime);
	}
}

void PhysicsManager::Render()
{
	for(int i = 0; i < (int)m_boxes.size(); ++i)
	{
		m_boxes[i]->Render();
	}
}

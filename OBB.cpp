#include "DXUT.h"
#include "OBB.h"


OBB::OBB(D3DXVECTOR3 pos, D3DXVECTOR3 size, bool dynamic)
{
	// Create Mesh
	m_pMesh = NULL;
	D3DXCreateBox( DXUTGetD3D9Device(), size.x, size.y, size.z, &m_pMesh, NULL);

	// Create Motion State
	btQuaternion q(0.0f,0.0f, 0.0f);
	btVector3 p(pos.x, pos.y, pos.z);
	btTransform startTrans( q, p);
	btMotionState* ms = new btDefaultMotionState(startTrans);

	// Create Collision Shape
	btCollisionShape* cs = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));

	// Create Rigid Body
	float mass = size.x * size.y * size.z;
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	cs->calculateLocalInertia( mass, localInertia);
	if(!dynamic)
		mass = 0.0f;

	m_Body = new btRigidBody( mass, ms, cs, localInertia);
}


OBB::~OBB(void)
{
	Release();
}

void OBB::Render()
{

}

void OBB::Release()
{
	SAFE_RELEASE(m_pMesh);

	// Rigid Body ‘› ±”…mgr Õ∑≈
}

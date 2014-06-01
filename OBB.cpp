#include "DXUT.h"
#include "OBB.h"
#include "PhysicsMgr.h"
#include "Utils.h"


OBB::OBB(D3DXVECTOR3 pos, D3DXVECTOR3 size, bool dynamic)
{
	D3DXQUATERNION q;
	D3DXQuaternionIdentity(&q);
	Init(pos, size, q, dynamic);
}

OBB::OBB(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXQUATERNION& q,bool dynamic/*=true*/)
{
	Init(pos, size, q, dynamic);
}

void OBB::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXQUATERNION& rot, bool dynamic)
{
	//Create Mesh
	m_pMesh = NULL;
	m_size = size;
	D3DXCreateBox( DXUTGetD3D9Device(), size.x, size.y, size.z, &m_pMesh, NULL);

	//Create Motion State
	D3DXQuaternionNormalize(&rot, &rot);
	m_orgRot = rot;
	btQuaternion q(rot.x, rot.y, rot.z, rot.w);
	btVector3 p(pos.x, pos.y, pos.z);
	btTransform startTrans(q, p);
	btMotionState *ms = new btDefaultMotionState(startTrans);

	//Create Collision Shape
	btCollisionShape *cs = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));

	//Create Rigid Body
	float mass = size.x * size.y * size.z;
	btVector3 localInertia(0,0,0);
	cs->calculateLocalInertia(mass,localInertia);
	if(!dynamic)mass = 0.0f;

	m_Body = new btRigidBody(mass, ms, cs, localInertia);

	m_Body->setDamping(0.05f, 0.85f);
	m_Body->setDeactivationTime(0.8f);
	m_Body->setSleepingThresholds(1.6f, 2.0f);
}


OBB::~OBB(void)
{
	Release();
}

void OBB::Render(const char* tech)
{
	btMotionState* ms = m_Body->getMotionState();
	if(!ms)
		return;

	g_pEffect->SetMatrix("g_mWorld", &BT2DX_MATRIX(*ms));

	D3DXHANDLE hTech = g_pEffect->GetTechniqueByName(tech);
	g_pEffect->SetTechnique(hTech);
	g_pEffect->Begin( NULL, NULL);
	g_pEffect->BeginPass(0);

	m_pMesh->DrawSubset(0);

	g_pEffect->EndPass();
	g_pEffect->End();
}

void OBB::Release()
{
	SAFE_RELEASE(m_pMesh);

	SAFE_DELETE(m_Body);

	// Rigid Body ��ʱ��mgr�ͷ�
}

void OBB::Update(float deltaTime)
{
	if (m_Body->wantsSleeping())
	{
		m_Body->updateDeactivation(deltaTime);
	}
}

D3DXVECTOR3 OBB::SetPivot(D3DXVECTOR3& pivot)
{
	btMotionState *ms = m_Body->getMotionState();
	if(ms == NULL)return D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXMATRIX world = BT2DX_MATRIX(*ms);
	D3DXVECTOR3 newPivot;
	D3DXMatrixInverse(&world, NULL, &world);
	D3DXVec3TransformCoord(&newPivot, &pivot, &world);

	return newPivot;
}

D3DXVECTOR3 OBB::GetPivot(D3DXVECTOR3& pivot)
{
	btMotionState *ms = m_Body->getMotionState();
	if(ms == NULL)return D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXMATRIX world = BT2DX_MATRIX(*ms);
	D3DXVECTOR3 newPivot;
	D3DXVec3TransformCoord(&newPivot, &pivot, &world);

	return newPivot;
}

D3DXQUATERNION OBB::GetRotation(D3DXQUATERNION orgBoneRot)
{
	btMotionState *ms = m_Body->getMotionState();
	btTransform t;
	ms->getWorldTransform(t);
	D3DXQUATERNION rot = BT2DX_QUATERNION(t.getRotation());

	D3DXQUATERNION invOrgRot;
	D3DXQuaternionInverse(&invOrgRot, &m_orgRot);
	D3DXQUATERNION diff = invOrgRot * rot;

	return orgBoneRot * diff;
}

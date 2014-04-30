#pragma once
#pragma warning (disable:4127)

#include <btBulletDynamicsCommon.h>
class OBB
{
public:
	OBB(D3DXVECTOR3 pos, D3DXVECTOR3 size, bool dynamic=true);
	OBB(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXQUATERNION& q,bool dynamic=true);
	~OBB(void);

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXQUATERNION& rot, bool dynamic);
	void Release();

	void Update(float deltaTime);
	void Render(const char* tech);

	D3DXVECTOR3 SetPivot(D3DXVECTOR3& pivot);
	D3DXVECTOR3 GetPivot(D3DXVECTOR3& pivot);
	D3DXQUATERNION GetRotation(D3DXQUATERNION orgBoneRot);

public:
	btRigidBody* m_Body;
	D3DXVECTOR3 m_size;
	D3DXQUATERNION m_orgRot;

private:
	ID3DXMesh* m_pMesh;
};


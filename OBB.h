#pragma once
#pragma warning (disable:4127)

#include <btBulletDynamicsCommon.h>
class OBB
{
public:
	OBB(D3DXVECTOR3 pos, D3DXVECTOR3 size, bool dynamic=true);
	~OBB(void);

	void Release();
	void Render();

public:
	btRigidBody* m_Body;

private:
	ID3DXMesh* m_pMesh;
};


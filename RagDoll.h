#pragma once
#pragma warning (disable:4189)
#include "skinnedmesh.h"
#include "OBB.h"
class RagDoll :
	public SkinnedMesh
{
public:
	RagDoll(WCHAR* fileName, D3DXMATRIX& world);
	~RagDoll(void);

	void InitBones(Bone* bone);
	void Release();
	void Update(float deltaTime);
	void Render(const char* tech);
	void UpdateSkeleton(Bone* bone);
	OBB* CreateBoneBox(Bone* parent, Bone* bone, D3DXVECTOR3 size, D3DXQUATERNION rot);
	void CreateHinge(Bone* parent, OBB* A, OBB* B, float upperLimit, float lowerLimit, D3DXVECTOR3& hingeAxisA, D3DXVECTOR3& hingeAxisB, bool ignoreCollisions = true);
	void CreateTwistCone(Bone* parent, OBB* A, OBB* B, float limit, D3DXVECTOR3& hingeAxisA, D3DXVECTOR3& hingeAxisB, bool ignoreCollisions = true);

private:
	std::vector<OBB*> m_boxes;
};


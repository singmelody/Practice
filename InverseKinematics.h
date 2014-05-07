#pragma once

class SkinnedMesh;
struct Bone;

class InverseKinematics
{
public:
	InverseKinematics(SkinnedMesh* pSkinnedMesh);
	~InverseKinematics(void);

	void UpdateHeadIK();
	void ApplyLookAtIK(D3DXVECTOR3& lookAtTarget, float maxAngle);

private:
	SkinnedMesh*	m_skinnedMesh;
	Bone*			m_headBone;
	D3DXVECTOR3		m_headForward;
};


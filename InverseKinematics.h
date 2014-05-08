#pragma once

class SkinnedMesh;
struct Bone;

class InverseKinematics
{
public:
	InverseKinematics(SkinnedMesh* pSkinnedMesh);
	~InverseKinematics(void);

	void UpdateHeadIK();
	void UpdateArmIK();
	void ApplyLookAtIK(D3DXVECTOR3& lookAtTarget, float maxAngle);
	void ApplyArmIK(D3DXVECTOR3 &hingeAxis, D3DXVECTOR3 &target);


private:
	SkinnedMesh*	m_skinnedMesh;
	Bone*			m_headBone;
	Bone*			m_shoulderBone;
	Bone*			m_elbowBone;
	Bone*			m_handBone;
	D3DXVECTOR3		m_headForward;
};


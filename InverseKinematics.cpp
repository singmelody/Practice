#include "DXUT.h"
#include "InverseKinematics.h"
#include "skinnedMesh.h"

InverseKinematics::InverseKinematics(SkinnedMesh* pSkinnedMesh)
{
	m_skinnedMesh = pSkinnedMesh;

	// Find the head bone
	m_headBone = (Bone*)m_skinnedMesh->GetBone("Head");

	if ( m_headBone )
	{
		// calculate the local forward vector for the head bone

		// remove translation from head matrix
		D3DXMATRIX headMatrix;
		headMatrix = m_headBone->CombinedTransformationMatrix;
		headMatrix._41 = 0.0f;
		headMatrix._42 = 0.0f;
		headMatrix._43 = 0.0f;
		headMatrix._44 = 1.0f;

		D3DXMATRIX toHeadSpace;
		if(D3DXMatrixInverse( &toHeadSpace, NULL, &headMatrix) == NULL)
			return;

		// the model is looking toward -z in the content
		D3DXVECTOR4 vec;
		D3DXVec3Transform( &vec, &D3DXVECTOR3(0.0f, 0.0f, -1.0f), &toHeadSpace);
		m_headForward = D3DXVECTOR3( vec.x, vec.y, vec.z);
	}
}


InverseKinematics::~InverseKinematics(void)
{
}

void InverseKinematics::ApplyLookAtIK(D3DXVECTOR3& lookAtTarget, float maxAngle)
{
	// start by transforming to local space
	D3DXMATRIX mtx2Local;
	D3DXMatrixInverse( &mtx2Local, NULL,
		&m_headBone->CombinedTransformationMatrix);

	D3DXVECTOR3 localLookAt;
	D3DXVec3TransformCoord(&localLookAt, &lookAtTarget, &mtx2Local);

	// normalize local look at target
	D3DXVec3Normalize( &localLookAt, &localLookAt);

	// Get rotation axis and angle
	D3DXVECTOR3 localRotationAxis;
	D3DXVec3Cross( &localRotationAxis, &m_headForward, &localLookAt);
	D3DXVec3Normalize( &localRotationAxis, &localRotationAxis);

	float localAngle  = acosf(D3DXVec3Dot( &m_headForward,
										   &localLookAt));

	// Limit angle
	localAngle = min( localAngle, maxAngle);

	// Apply the transformation to the bone
	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis( &rotation, &localRotationAxis, localAngle);
	m_headBone->CombinedTransformationMatrix = rotation * m_headBone->CombinedTransformationMatrix;

	// Update changes to child bones
	if(m_headBone->pFrameFirstChild)
	{
		m_skinnedMesh->UpdateMatrices(
			(Bone*)m_headBone->pFrameFirstChild,
			&m_headBone->CombinedTransformationMatrix);
	}
}

void InverseKinematics::UpdateHeadIK()
{
	if(!m_headBone)
		return;

	// Get look at point from cursor pos
	POINT p;
	GetCursorPos(&p);

	p.x = max(min(p.x, 640), 0);
	p.y = max(min(p.y, 480), 0);
	float x = (p.x - 320) / 640.0f;
	float y = (p.y - 240) / 480.0f;	

	D3DXVECTOR3 lookAt(x, 1.5f - y, -1.0f);

	// Restrict head movement to 60 degrees
	ApplyLookAtIK(lookAt, D3DXToRadian(60.0f));
}

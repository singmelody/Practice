#include "DXUT.h"
#include "InverseKinematics.h"
#include "skinnedMesh.h"

InverseKinematics::InverseKinematics(SkinnedMesh* pSkinnedMesh)
{
	m_skinnedMesh = pSkinnedMesh;

	// Find the head bone
	m_headBone = (Bone*)m_skinnedMesh->GetBone("Head");
	m_shoulderBone = (Bone*)m_skinnedMesh->GetBone("Bone20");
	m_elbowBone = (Bone*)m_skinnedMesh->GetBone("Bone21");
	m_handBone = (Bone*)m_skinnedMesh->GetBone("Bone27"); 

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

void InverseKinematics::UpdateArmIK()
{
	if(m_shoulderBone == NULL || m_elbowBone == NULL || m_handBone == NULL)
		return;

	// Get look at point from cursor pos
	POINT p;
	GetCursorPos(&p);

	p.x = max(min(p.x, 640), 0);
	p.y = max(min(p.y, 480), 0);
	float x = (p.x - 320) / 640.0f;
	float y = (p.y - 240) / 480.0f;	

	D3DXVECTOR3 lookAt(x, 1.5f - y, -1.0f);

	// Grab a point located at (-0.5, 1.5, 0.0)
	// The elbow joint axis (0, 0, -1) in content
	ApplyArmIK(D3DXVECTOR3(0, 0, -1), lookAt);
}

void InverseKinematics::ApplyArmIK(D3DXVECTOR3 &hingeAxis, D3DXVECTOR3 &target)
{
	// Set up some vectors and position
	D3DXVECTOR3 startPos = D3DXVECTOR3(
		m_shoulderBone->CombinedTransformationMatrix._41,
		m_shoulderBone->CombinedTransformationMatrix._42,
		m_shoulderBone->CombinedTransformationMatrix._43
		);

	D3DXVECTOR3 jointPos = D3DXVECTOR3(
		m_elbowBone->CombinedTransformationMatrix._41,
		m_elbowBone->CombinedTransformationMatrix._42,
		m_elbowBone->CombinedTransformationMatrix._43
		);

	D3DXVECTOR3 endPos = D3DXVECTOR3(
		m_handBone->CombinedTransformationMatrix._41,
		m_handBone->CombinedTransformationMatrix._42,
		m_handBone->CombinedTransformationMatrix._43
		);

	D3DXVECTOR3 start2Target = target - startPos;
	D3DXVECTOR3 start2Joint = jointPos - startPos;
	D3DXVECTOR3 joint2End = endPos - jointPos;

	float distStart2Target = D3DXVec3Length(&start2Target);
	float distStart2Joint = D3DXVec3Length(&start2Joint);
	float distJoint2End = D3DXVec3Length(&joint2End);

	// Calculate joint bone rotation
	// calcuate  current angle and wanted angle
	float wantedJointAngle = 0.0f;

	if ( start2Target  >= start2Joint + joint2End )
	{
		// straingt arm
		wantedJointAngle = D3DXToRadian(180.0f);
	}
	else
	{
		// calculate wanted joint angle( using the law of cos)
		float cosAngle = ( distStart2Joint * distStart2Joint + distJoint2End * distJoint2End - distStart2Target * distStart2Target) / (2.0f * distStart2Joint * distJoint2End) ;
		wantedJointAngle = acosf(cosAngle);
	}

	// Normalized vectors
	D3DXVECTOR3 nmlStart2Joint = start2Joint;
	D3DXVECTOR3 nmlJoint2End = joint2End;
	D3DXVec3Normalize( &nmlStart2Joint, &nmlStart2Joint);
	D3DXVec3Normalize( &nmlJoint2End, &nmlJoint2End);

	// Calculate the current joint angle
	float currentJointAngle = acosf( D3DXVec3Dot( &(-nmlStart2Joint), &nmlJoint2End) );

	// Calculate rotation matrix
	float diffJointAngle = wantedJointAngle - currentJointAngle;
	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis( &rotation, &hingeAxis, diffJointAngle);

	// Apply elbow transformation
	m_elbowBone->TransformationMatrix = rotation * m_elbowBone->TransformationMatrix;

	// Now the elbow "bending" has been done, Next you just
	// need to rotate the shoulder using the look at ik algorithm

	// Calcuate new end pos
	// calcuate this in world pos & transform
	// it later 2 start bone local space
	D3DXMATRIX tempMatrix;
	tempMatrix = m_elbowBone->CombinedTransformationMatrix;
	tempMatrix._41 = 0.0f; 
	tempMatrix._42 = 0.0f; 
	tempMatrix._43 = 0.0f; 
	tempMatrix._44 = 1.0f;

	D3DXVECTOR3 worldHingeAxis;
	D3DXVECTOR3 newJoint2End;

	D3DXVec3TransformCoord( &worldHingeAxis, &hingeAxis, &tempMatrix);
	D3DXMatrixRotationAxis( &rotation, &worldHingeAxis, diffJointAngle);
	D3DXVec3TransformCoord( &newJoint2End, &joint2End, &rotation);

	D3DXVECTOR3 newEndPos;
	D3DXVec3Add( &newEndPos, &newJoint2End, &jointPos);

	// Calculate start bone rotation
	D3DXMATRIX mtx2Local;
	D3DXMatrixInverse( &mtx2Local, NULL, &m_shoulderBone->CombinedTransformationMatrix);

	D3DXVECTOR3 localNewEnd;
	D3DXVECTOR3 localTarget;
	D3DXVec3TransformCoord( &localNewEnd, &newEndPos, &mtx2Local);
	D3DXVec3TransformCoord( &localTarget, &target, &mtx2Local);
	D3DXVec3Normalize( &localNewEnd, &localNewEnd);
	D3DXVec3Normalize( &localTarget, &localTarget);

	D3DXVECTOR3 localAxis;
	D3DXVec3Cross( &localAxis, &localNewEnd, &localTarget);
	if(D3DXVec3Length(&localAxis) == 0.0f)
		return;

	D3DXVec3Normalize( &localAxis, &localAxis);
	float localAngle = acosf( D3DXVec3Dot( &localNewEnd, &localTarget));

	// Apply the rotation that makes the bone turn
	D3DXMatrixRotationAxis( &rotation, &localAxis, localAngle);
	m_shoulderBone->CombinedTransformationMatrix = rotation * m_shoulderBone->CombinedTransformationMatrix;
	m_shoulderBone->TransformationMatrix = rotation * m_shoulderBone->TransformationMatrix;

	// update matrix of child bones
	if (m_shoulderBone->pFrameFirstChild)
	{
		m_skinnedMesh->UpdateMatrices( (Bone*)m_shoulderBone->pFrameFirstChild,
										&m_shoulderBone->CombinedTransformationMatrix);
	}
}

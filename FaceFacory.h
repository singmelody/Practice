#pragma once
#include <DXUT.h>
#include <vector>

class FaceModel;

class FaceFacory
{
public:
	FaceFacory(const char* fileName);
	~FaceFacory(void);

	FaceModel* GenerateRandomFace();

private:
	void ExtractMeshes(D3DXFRAME* frame);
	ID3DXMesh* CreateMorphTarget(ID3DXMesh* mesh, std::vector<float>& morphWeights);

private:
	ID3DXMesh* m_baseMesh;
	ID3DXMesh* m_blinkMesh;
	ID3DXMesh* m_eyeMesh;

	std::vector<ID3DXMesh*> m_emotionMeshes;
	std::vector<ID3DXMesh*>	m_speechMeshes;
	std::vector<ID3DXMesh*>	m_morphMeshes;

	std::vector<IDirect3DTexture9*> m_faceTexes;
};


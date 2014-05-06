#pragma once

#include "Eye.h"
#include <vector>

class Face
{
public:
	Face(void);
	~Face(void);

	void Init();
	void Update(float deltaTime);
	void Render(const char* tech);
	void RenderEye(const char* tech, D3DXVECTOR3 pos);

private:
	ID3DXMesh*			m_faceMesh;
	IDirect3DTexture9*	m_faceTexture;

	Eye		m_eyes[2];
};

class ComplexFace
{
public:
	ComplexFace(const std::string& filename);
	~ComplexFace();

	void Update(float deltaTime);
	void Render(const char* tech);
	void ExtractMeshes(D3DXFRAME* frame);

private:
	ID3DXMesh* m_baseMesh;
	ID3DXMesh* m_binkMesh;
	std::vector<ID3DXMesh*> m_emotionMeshes;
	std::vector<ID3DXMesh*>	m_speechMeshes;

	IDirect3DVertexDeclaration9* m_pFaceVertexDecl;
	IDirect3DTexture9*			 m_FaceTexture;
	D3DXVECTOR4					 m_morphWeights;

	Eye							 m_eyes[2];
};
#pragma once

#include "Eye.h"
#include <vector>

class FaceController;

class Face
{
public:
	Face(void);
	~Face(void);

	void Init();
	void Update(float deltaTime);
	void Render(const char* tech);
	void RenderEye(const char* tech, D3DXVECTOR3 pos);

public:
	ID3DXMesh*			m_faceMesh;
	IDirect3DTexture9*	m_faceTexture;

	Eye		m_eyes[2];
};

class ComplexFace
{
public:
	ComplexFace();
	ComplexFace(const std::string& filename);
	~ComplexFace();

	void Update(float deltaTime);
	void Render(const char* tech);
	void ExtractMeshes(D3DXFRAME* frame);
public:
	ID3DXMesh* m_baseMesh;
	ID3DXMesh* m_binkMesh;
	std::vector<ID3DXMesh*> m_emotionMeshes;
	std::vector<ID3DXMesh*>	m_speechMeshes;

	IDirect3DVertexDeclaration9* m_pFaceVertexDecl;
	IDirect3DTexture9*			 m_FaceTexture;
	D3DXVECTOR4					 m_morphWeights;

	Eye							 m_eyes[2];
};

class FaceModel
{
public:
	FaceModel();
	FaceModel(const char* filename);

	~FaceModel();

	void Render(FaceController* pController);
	void ExtractMeshes(D3DXFRAME* frame);
	void SetStreamSources(FaceController *pController);

public:
	void AddTangent(ID3DXMesh** pMesh);

	ID3DXMesh* m_baseMesh;
	ID3DXMesh* m_binkMesh;
	std::vector<ID3DXMesh*> m_emotionMeshes;
	std::vector<ID3DXMesh*>	m_speechMeshes;
	IDirect3DVertexDeclaration9* m_pFaceVertexDecl;
	IDirect3DTexture9*		m_faceTex;
	IDirect3DTexture9*		m_normalTex;
	IDirect3DTexture9*		m_specularTex;
	IDirect3DTexture9*		m_wrinkleTex;
};
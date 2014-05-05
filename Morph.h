#pragma once
#include "DXUT.h"
#include "skinnedMesh.h"

class Morph
{
public:
	Morph(void);
	~Morph(void);

	void Init();
	void Update();
	void Render(const char* tech);

	float GetBlend() { return m_blend; }
	void SetBlend(float blend) { m_blend = blend; }

private:
	ID3DXMesh* m_Target01,*m_Target02, *m_face;
	float m_blend;
};

class MultiMorph
{
public:
	MultiMorph();
	~MultiMorph();

	void Init();
	void Update(float deltaTime);
	void Render(const char* tech);

private:
	void LoadMesh(const char* c, ID3DXMesh** mesh);
	ID3DXMesh* m_baseMesh;
	ID3DXMesh* m_targets[4];

	IDirect3DVertexDeclaration9* m_decl;

	D3DXVECTOR4 m_oldWeights, m_newWeights;
	float m_prc;
};

class MorphMesh : public SkinnedMesh
{
	friend class Application;
public:
	MorphMesh();
	~MorphMesh();

	void Init();
	void Update(float deltaTime);
	void Render();
	void RenderHuman(Bone *bone);

private:	
	void LoadTex(const char* c, IDirect3DTexture9** mesh);
	ID3DXAnimationController* m_animControl;
	IDirect3DVertexDeclaration9 *m_pDecl;

	IDirect3DTexture9* m_pHumanTexture;
	IDirect3DTexture9* m_pWolfTexture;
};
 


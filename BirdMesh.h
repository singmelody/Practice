#pragma once
#include <vector>

class BirdMesh
{
public:
	BirdMesh(void);
	BirdMesh(const char* fileName);

	~BirdMesh(void);

	HRESULT Load(const char* fileName);
	void Render();
	void Release();

private:
	ID3DXMesh* m_mesh;
	std::vector<IDirect3DTexture9*> m_textures;
	std::vector<D3DMATERIAL9> m_materials;
	D3DMATERIAL9	m_white;
};


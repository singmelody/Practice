#pragma once
#include <vector>
class Eye
{
public:
	Eye();
	~Eye();
	void Init(D3DXVECTOR3 position, int textureIndex);
	void Init(D3DXVECTOR3 position);
	void Render();
	void Render(D3DXMATRIX &headMatrix);
	void LookAt(D3DXVECTOR3 focus);

private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_lookAt;
	D3DXMATRIX m_rotation;

	static ID3DXMesh* sm_EyeMesh;
	static std::vector<IDirect3DTexture9*> sm_EyeTextures;
	int m_textureIndex;
};

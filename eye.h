#pragma once

class Eye
{
public:
	Eye();
	~Eye();
	void Init(D3DXVECTOR3 position);
	void Render();
	void LookAt(D3DXVECTOR3 focus);

private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_lookAt;
	D3DXMATRIX m_rotation;

	ID3DXMesh *m_EyeMesh;
	IDirect3DTexture9 *m_EyeTexture;
};

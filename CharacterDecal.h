#pragma once

#include <d3dx9.h>
#include <map>

using namespace std;

class CharacterDecal
{
public:
	CharacterDecal(ID3DXMesh* pDecalMesh);
	~CharacterDecal();
	void Render();

public:
	ID3DXMesh* m_pDecalMesh;
	static IDirect3DTexture9* sm_pDecalTexture;
};
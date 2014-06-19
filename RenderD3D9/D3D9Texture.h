#pragma once
#include "ITexture.hpp"

namespace Dream
{

class D3D9Texture : public ITexture
{
public:
	D3D9Texture();
	virtual ~D3D9Texture();
};

}
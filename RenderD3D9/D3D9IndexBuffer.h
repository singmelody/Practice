#pragma once
#include "IIndexBuffer.hpp"

namespace Dream
{

class D3D9IndexBuffer : public IIndexBuffer
{
public:
	D3D9IndexBuffer(void);
	virtual ~D3D9IndexBuffer(void);
};

}
#pragma once

namespace Dream
{
class IIndexBuffer;
class IVertexBuffer;
class ITexture;
class IVertexLayout;


class IRenderDevice
{
public:
	enum SHADERPARAM
	{
		MATRIX,

		INVALID
	};

	IRenderDevice(){}
	virtual ~IRenderDevice(){}

	virtual bool Init(const void* wnd) = 0;
	virtual void Destroy() = 0;

	virtual bool CheckCaps() = 0;
	virtual bool CreateDevice() = 0;

	virtual bool BeginScene() = 0;
	virtual void EndScene() = 0;
	
	virtual void Draw(size_t startVertex,size_t vertexCount,size_t startIdx,size_t idxCount) = 0;

	virtual void Present() = 0;

	virtual void SetShaderParam(const char* name, void* data, SHADERPARAM type) = 0;
	virtual void SetVertexLayout(const IVertexLayout* layout) = 0;
	virtual void SetVertexBuffer(int streamIndex, const IVertexBuffer* vb, int offsetIdx) = 0;
	virtual void SetIndexBuffer(const IIndexBuffer* ib) = 0;

	virtual ITexture* CreateTexture(const char* name) = 0;
};

}


#pragma once
class IRenderer
{
public:
	IRenderer(void);
	virtual ~IRenderer(void);

	virtual bool CreateDevice() = 0;

	virtual bool CheckCaps() = 0;

	virtual bool CreateSwapChain() = 0;

	virtual UINT GetReference() = 0;

	virtual bool CreateRenderTargetView() = 0;

	virtual bool CreateDepthStencilBuffer() = 0;

	virtual bool BindRTAndDepthToMS() = 0;

	virtual bool CreateViewPort() = 0;

	virtual bool ShaderParse() = 0;
};


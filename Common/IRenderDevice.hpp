#pragma once

namespace Dream
{
class ITexture;

class IRenderDevice
{
public:
	IRenderDevice(){}
	virtual ~IRenderDevice(){}

	virtual bool CreateDevice(const void* mainWnd) = 0;

	virtual bool CheckCaps() = 0;

	virtual bool CreateSwapChain() = 0;

	virtual int GetReference() = 0;

	virtual bool CreateRenderTargetView() = 0;

	virtual bool CreateDepthStencilBuffer() = 0;

	virtual bool BindRTAndDepthToMS() = 0;

	virtual bool CreateViewPort() = 0;

	virtual bool ShaderParse() = 0;

	virtual ITexture* CreateTexture(const char* name) = 0;

};

}

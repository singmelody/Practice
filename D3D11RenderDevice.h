#pragma once
#include "IRenderDevice.h"
#include <D3DX11.h>

class D3D11RenderDevice :
	public IRenderDevice
{
public:
	D3D11RenderDevice(void);
	virtual ~D3D11RenderDevice(void);

	void Release();

	bool CreateDevice();

	bool CheckCaps();

	bool CreateSwapChain();

	bool CreateRenderTargetView();

	bool CreateDepthStencilBuffer();

	bool BindRTAndDepthToMS();

	bool CreateViewPort();

	UINT GetReference();

private:
	ID3D11Device*			m_d3d11Device;
	D3D_FEATURE_LEVEL*		m_featureLevel;
	ID3D11DeviceContext*	m_d3d11DeviceContext;
	IDXGISwapChain*			m_swapChain;
	UINT					m_m4xMsaaQuality;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11DepthStencilView* m_depthStencilView;
};


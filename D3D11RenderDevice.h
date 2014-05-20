#pragma once
#include "IRenderDevice.h"
#include <D3DX11.h>
#include <inc/d3dx11effect.h>


class D3D11RenderDevice :
	public IRenderDevice
{
public:



	void Release();

	bool CreateDevice();

	bool CheckCaps();

	bool CreateSwapChain();

	bool CreateRenderTargetView();

	bool CreateDepthStencilBuffer();

	bool BindRTAndDepthToMS();

	bool CreateViewPort();

	UINT GetReference();

	//--------- temp
	static D3D11RenderDevice& Instance();

	bool CreateVertexDecl();

	bool ShaderParse();

	bool Op();

	D3D11RenderDevice(void);
	virtual ~D3D11RenderDevice(void);

protected:


	static D3D11RenderDevice m_Instance;
public:
	ID3D11Device*			m_d3d11Device;
	D3D_FEATURE_LEVEL*		m_featureLevel;
	ID3D11DeviceContext*	m_d3d11DeviceContext;
	IDXGISwapChain*			m_swapChain;
	UINT					m_m4xMsaaQuality;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11DepthStencilView* m_depthStencilView;

	//--------- temp
	ID3DX11Effect*			m_fx;
};


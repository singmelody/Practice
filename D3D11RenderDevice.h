#pragma once
#include "IRenderDevice.h"
#include <D3DX11.h>
#include <xnamath.h>
#include <inc/d3dx11effect.h>
#include "Effects.h"
#include "BlurFilter.h"

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
	struct SimpleVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

	static D3D11RenderDevice& Instance();

	bool CreateVertexDecl();

	bool CreateGBuffer();

	bool ShaderParse();

	bool Render();

	bool RenderCube();

	bool RenderSpriteTree();

	bool RenderScreen();

	bool RenderTessellation();

	bool RenderTriangleTessellation();

	bool BuildTreeSpritesBuffer();

	bool BuildCubeBuffer();

	bool BuildBuffersAndViews();

	bool BuildLight();

	bool BuildOffscreenViews();

	bool BuildScreenGeometryBuffers();

	bool BuildQuadPatchBuffer();

	bool BuildTriPatchBuffers();

	void DoComputeWork();

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
	ID3DX11Effect*			m_basicFx;
	ID3DX11Effect*			m_gsFx;
	ID3DX11Effect*			m_csFx;
	ID3DX11Effect*			m_blurFx;
	ID3DX11Effect*			m_tsFx;
	ID3DX11Effect*			m_triTsFx;

	ID3DX11EffectTechnique* m_tech;
	ID3DX11EffectTechnique* m_gsTech;
	ID3DX11EffectTechnique* m_csTech;

	ID3DX11EffectMatrixVariable* m_fxWorldViewProj;
	ID3D11InputLayout*		m_vertexDesc;
	ID3D11Buffer*			m_vertexBuff;
	ID3D11Buffer*			m_indicesBuff;

	static const UINT TreeCount;

	ID3D11Buffer*			m_TreeSpritesVB;
	TreeSpriteEffect*		m_treeEffect;
	VecAddEffect*			m_vecAddEffect;
	BasicEffect*			m_basicEffect;
	TessellationEffect*		m_tsEffect;
	TessellationEffect*		m_triEffect;

	BlurFilter*				m_blurFilter;

	DirectionalLight		m_DirLights[3];
	Material				m_TreeMat;
	XMFLOAT3				m_EyePosW;
	bool					m_AlphaToCoverageOn;
	ID3D11ShaderResourceView* m_TreeTextureMapArraySRV;

	XMMATRIX				m_View;
	XMMATRIX				m_Proj; 

	ID3D11ShaderResourceView* m_InputASRV;
	ID3D11ShaderResourceView* m_InputBSRV;
	ID3D11UnorderedAccessView* m_OutputUAV;

	ID3D11Buffer* m_OutputBuffer;
	ID3D11Buffer* m_OutputDebugBuffer;

	UINT		  m_NumElements;

	ID3D11ShaderResourceView* m_OffscreenSRV;
	ID3D11UnorderedAccessView* m_OffscreenUAV;
	ID3D11RenderTargetView* m_OffscreenRTV;

	ID3D11Buffer* m_ScreenQuadVB;
	ID3D11Buffer* m_ScreenQuadIB;
	ID3D11Buffer* m_QuadPatchVB;
	ID3D11Buffer* m_TriPatchVB;
};


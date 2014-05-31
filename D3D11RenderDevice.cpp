#include "DXUT.h"
#include "D3D11RenderDevice.h"
#include "Utils.h"
#include "Vertex.h"
#include "MathHelper.h"
#include "RenderStates.h"
#include <fstream>
#include "GeometryGenerator.h"

struct Data
{
	XMFLOAT3 v1;
	XMFLOAT2 v2;
};


namespace Colors
{
	XMGLOBALCONST XMVECTORF32 White     = {1.0f, 1.0f, 1.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Black     = {0.0f, 0.0f, 0.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Red       = {1.0f, 0.0f, 0.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Green     = {0.0f, 1.0f, 0.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Blue      = {0.0f, 0.0f, 1.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Yellow    = {1.0f, 1.0f, 0.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Cyan      = {0.0f, 1.0f, 1.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Magenta   = {1.0f, 0.0f, 1.0f, 1.0f};

	XMGLOBALCONST XMVECTORF32 Silver    = {0.75f, 0.75f, 0.75f, 1.0f};
	XMGLOBALCONST XMVECTORF32 LightSteelBlue = {0.69f, 0.77f, 0.87f, 1.0f};
}



const UINT D3D11RenderDevice::TreeCount = 16;

D3D11RenderDevice::D3D11RenderDevice(void)
	: m_d3d11Device(NULL),
	m_d3d11DeviceContext(NULL),
	m_featureLevel(new D3D_FEATURE_LEVEL()),
	m_m4xMsaaQuality(NULL),
	m_swapChain(NULL),
	m_renderTargetView(NULL),
	m_depthStencilView(NULL),
	m_fx(NULL),
	m_gsFx(NULL),
	m_csFx(NULL),
	m_TreeSpritesVB(NULL),
	m_treeEffect(NULL),
	m_AlphaToCoverageOn(true),
	m_TreeTextureMapArraySRV(NULL),
	m_InputASRV(NULL), 
	m_InputBSRV(NULL), 
	m_OutputUAV(NULL),
	m_OutputBuffer(NULL),
	m_OutputDebugBuffer(NULL),
	m_NumElements(32),
	m_blurFilter(NULL),
	m_blurFx(NULL),
	m_OffscreenSRV(NULL),
	m_OffscreenUAV(NULL),
	m_OffscreenRTV(NULL),
	m_basicEffect(NULL),
	m_basicFx(NULL),
	m_ScreenQuadVB(NULL),
	m_ScreenQuadIB(NULL),
	m_tsFx(NULL),
	m_tsEffect(NULL),
	m_QuadPatchVB(NULL),
	m_triEffect(NULL),
	m_triTsFx(NULL),
	m_TriPatchVB(NULL)
{
}


D3D11RenderDevice::~D3D11RenderDevice(void)
{
	Release();
}


void D3D11RenderDevice::Release()
{
	SAFE_RELEASE(m_vertexDesc);
	SAFE_RELEASE(m_vertexBuff);
	SAFE_RELEASE(m_indicesBuff);
	SAFE_DELETE(m_featureLevel);
	SAFE_RELEASE(m_depthStencilView);
	SAFE_RELEASE(m_renderTargetView);
	SAFE_RELEASE(m_swapChain);
	SAFE_RELEASE(m_d3d11DeviceContext);
	SAFE_RELEASE(m_fx);
	SAFE_RELEASE(m_gsFx);
	SAFE_RELEASE(m_csFx)
	SAFE_RELEASE(m_TreeSpritesVB);
	SAFE_RELEASE(m_TreeTextureMapArraySRV);
	SAFE_DELETE(m_treeEffect);
	SAFE_DELETE(m_vecAddEffect);
	SAFE_RELEASE(m_InputASRV);
	SAFE_RELEASE(m_InputBSRV);
	SAFE_RELEASE(m_OutputUAV);
	SAFE_RELEASE(m_OutputBuffer);
	SAFE_RELEASE(m_OutputDebugBuffer);
	SAFE_DELETE(m_blurFilter);
	SAFE_RELEASE(m_blurFx);
	SAFE_RELEASE(m_OffscreenRTV);
	SAFE_RELEASE(m_OffscreenSRV);
	SAFE_RELEASE(m_OffscreenUAV);
	SAFE_RELEASE(m_basicFx);
	SAFE_DELETE(m_basicEffect);
	SAFE_RELEASE(m_ScreenQuadIB);
	SAFE_RELEASE(m_ScreenQuadVB);
	SAFE_DELETE(m_treeEffect);
	SAFE_RELEASE(m_tsFx);
	SAFE_DELETE(m_tsEffect);
	SAFE_RELEASE(m_QuadPatchVB);
	SAFE_DELETE(m_triEffect);
	SAFE_RELEASE(m_triTsFx);
	SAFE_RELEASE(m_TriPatchVB);

	RenderStates::DestroyAll();
	InputLayouts::DestroyAll();
	Effects::DestroyAll();
}


bool D3D11RenderDevice::CreateDevice()
{
	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[6] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	HRESULT hr = D3D11CreateDevice( 
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		featureLevels,
		sizeof(featureLevels)/sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,
		&m_d3d11Device,
		m_featureLevel,
		&m_d3d11DeviceContext
		);

	if(FAILED(hr))
	{
		printf("D3D11CreateDevice Failed\n");
		return false;
	}

	if( *m_featureLevel != D3D_FEATURE_LEVEL_11_0 )
	{
		printf("D3D_FEATURE_LEVEL_11_0 not supported\n");
		return false;
	}

	return true;
}

bool D3D11RenderDevice::CheckCaps()
{
	HRESULT hr;

	V( m_d3d11Device->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_m4xMsaaQuality));
	
	return m_m4xMsaaQuality > 0;
}

bool D3D11RenderDevice::CreateSwapChain()
{
	bool mEnable4xMsaa = false;
	HRESULT hr;

	IDXGIDevice* dxgiDevice = NULL;
	V(m_d3d11Device->QueryInterface( __uuidof(IDXGIDevice), (void**)&dxgiDevice));
	
	IDXGIAdapter* dxgiAdapter = NULL;
	V(dxgiDevice->GetParent( __uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = NULL;
	V(dxgiAdapter->GetParent( __uuidof(IDXGIFactory), (void**)&dxgiFactory));

	DXGI_SWAP_CHAIN_DESC desc;
	desc.BufferDesc.Width = 640;
	desc.BufferDesc.Height = 480;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	if( mEnable4xMsaa )
	{
		desc.SampleDesc.Count = 4;	// todo msaa check
		desc.SampleDesc.Quality = m_m4xMsaaQuality - 1;
	}
	else
	{
		desc.SampleDesc.Count = 1;	// todo msaa check
		desc.SampleDesc.Quality = 0;
	}
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = DXUTGetHWND();
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;


	V(dxgiFactory->CreateSwapChain( m_d3d11Device, &desc, &m_swapChain));

	SAFE_RELEASE(dxgiDevice);
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiFactory);

	return true;
}

UINT D3D11RenderDevice::GetReference()
{
	Release();
	UINT reference = m_d3d11Device->Release();
	
	return reference;
}

bool D3D11RenderDevice::CreateRenderTargetView()
{
	ID3D11Texture2D* backBuffer;
	m_swapChain->GetBuffer( NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	HRESULT hr = m_d3d11Device->CreateRenderTargetView( backBuffer, 0, &m_renderTargetView);
	SAFE_RELEASE(backBuffer);

	if( FAILED( hr ) )
		return false;

	return true;
}

bool D3D11RenderDevice::CreateDepthStencilBuffer()
{
	bool mEnable4xMsaa = false;
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = 640;
	desc.Height= 480;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if( mEnable4xMsaa )
	{
		desc.SampleDesc.Count = 4;
		desc.SampleDesc.Quality = m_m4xMsaaQuality - 1;
	}
	else
	{
		desc.SampleDesc.Count   = 1;
		desc.SampleDesc.Quality = 0;	
	}

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ID3D11Texture2D* depthStencilBuffer;
	HRESULT hr = m_d3d11Device->CreateTexture2D( &desc, 0, &depthStencilBuffer);
	if(FAILED(hr))
		return false;

	hr = m_d3d11Device->CreateDepthStencilView( depthStencilBuffer, NULL, &m_depthStencilView);
	if(FAILED(hr))
		return false;

	SAFE_RELEASE(depthStencilBuffer);

	return true;
}

bool D3D11RenderDevice::BindRTAndDepthToMS()
{
	m_d3d11DeviceContext->OMSetRenderTargets( 1, &m_renderTargetView, m_depthStencilView);
	return true;
}

bool D3D11RenderDevice::CreateViewPort()
{
	D3D11_VIEWPORT vp;
	vp.Width	= 640;
	vp.Height	= 480;
	vp.MaxDepth	= 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_d3d11DeviceContext->RSSetViewports( 1, &vp);

	return true;
}

bool D3D11RenderDevice::CreateVertexDecl()
{
	D3D11_INPUT_ELEMENT_DESC desc[] = 
	{
		"POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA, 0,
		"COLOR",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA, 0,
	};

	D3DX11_PASS_DESC passDesc;
	m_tech->GetPassByIndex(0)->GetDesc(&passDesc);

	HRESULT hr = m_d3d11Device->CreateInputLayout( desc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_vertexDesc);

	if(FAILED(hr))
		return false;

	return true;
}

bool D3D11RenderDevice::Render()
{
// 	// Render to our offscreen texture.  Note that we can use the same depth/stencil buffer
// 	// we normally use since our offscreen texture matches the dimensions.  
// 	ID3D11RenderTargetView* renderTargets[1] = {m_OffscreenRTV};
// 	m_d3d11DeviceContext->OMSetRenderTargets(1, renderTargets, m_depthStencilView);

	//--------------------------- Blur Up -------------------------------

	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	m_d3d11DeviceContext->ClearRenderTargetView( m_renderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
	m_d3d11DeviceContext->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	XMFLOAT4X4 mWorld;

	// Set constants
	// Build the view matrix.
	XMVECTOR pos    = XMVectorSet( -43.0f, 23.0f, -63.3f, 1.0f );
	XMVECTOR target = XMVectorZero();
	XMVECTOR up     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMFLOAT4X4 mView11;

	m_EyePosW = XMFLOAT3( -43.0f, 23.0f, -63.3f);

	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView11, V);

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mWorld, I);
	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	m_View  = V;
	m_Proj  = XMMatrixPerspectiveFovLH(0.25f*D3DX_PI, 640/(float)480.0f, 1.0f, 1000.0f);
	XMMATRIX worldViewProj = world*m_View*m_Proj;

	m_fxWorldViewProj->SetMatrix( reinterpret_cast<float*>(&worldViewProj));

	RenderSpriteTree();

	RenderCube();

	RenderTessellation();

	RenderTriangleTessellation();

// 	//------------------------------------Blur Below---------------------------------
// 	//
// 	// Restore the back buffer.  The offscreen render target will serve as an input into
// 	// the compute shader for blurring, so we must unbind it from the OM stage before we
// 	// can use it as an input into the compute shader.
// 	//
// 	renderTargets[0] = m_renderTargetView;
// 	m_d3d11DeviceContext->OMSetRenderTargets(1, renderTargets, m_depthStencilView);
// 
// 	//m_blurFilter->SetGaussianWeights(4.0f);
// 	m_blurFilter->BlurInPlace( m_d3d11DeviceContext, m_OffscreenSRV, m_OffscreenUAV, 4);
// 
// 	//
// 	// Draw fullscreen quad with texture of blurred scene on it.
// 	//
// 
// 	m_d3d11DeviceContext->ClearRenderTargetView( m_renderTargetView, ClearColor);
// 	m_d3d11DeviceContext->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
// 
// 	RenderScreen();

	HRESULT hr = m_swapChain->Present( 0, 0);
	if(FAILED(hr))
		return false;

	return true;
}

bool D3D11RenderDevice::ShaderParse()
{
	bool result = LoadShader("Color.fx", m_fx);

	if(!result)
		return false;

	m_tech = m_fx->GetTechniqueByName("ColorTech");


	result = LoadShader("TreeSprite.fx", m_gsFx);

	if(!result)
		return false;

	m_gsTech = m_gsFx->GetTechniqueByName("Light3TexAlphaClipFog");
	m_treeEffect = new TreeSpriteEffect(m_gsFx);

	result = LoadShader("Basic.fx", m_basicFx);
	if(!result)
		return false;

	m_basicEffect = new BasicEffect(m_basicFx);

	result = LoadShader("Tessellation.fx", m_tsFx);
	if(!result)
		return false;

	m_tsEffect = new TessellationEffect(m_tsFx);

	result = LoadShader("TriTessellation.fx", m_triTsFx);
	if(!result)
		return false;

	m_triEffect = new TessellationEffect(m_triTsFx);


	result = LoadShader("VecAdd.fx", m_csFx);
	if(!result)
		return false;

	m_csTech = m_csFx->GetTechniqueByName("VecAdd");
	m_vecAddEffect = new VecAddEffect(m_csFx);

	result = LoadShader("Blur.fx", m_blurFx);
	if(!result)
		return false;

	m_blurFilter = new BlurFilter();
	m_blurFilter->Init(m_d3d11Device, 640, 480, DXGI_FORMAT_R8G8B8A8_UNORM, m_blurFx);

	m_fxWorldViewProj = m_fx->GetVariableByName("gWorldViewProj")->AsMatrix();


	std::vector<std::string> treeFilenames;
	treeFilenames.push_back("meshes\\tree0.dds");
	treeFilenames.push_back("meshes\\tree1.dds");
	treeFilenames.push_back("meshes\\tree2.dds");
	treeFilenames.push_back("meshes\\tree3.dds");

	m_TreeTextureMapArraySRV = CreateTexture2DArraySRV(
		m_d3d11Device, m_d3d11DeviceContext, treeFilenames, DXGI_FORMAT_R8G8B8A8_UNORM);

	return true;
}

D3D11RenderDevice& D3D11RenderDevice::Instance()
{
	return m_Instance;
}

bool D3D11RenderDevice::CreateGBuffer()
{
	BuildCubeBuffer();

	BuildTreeSpritesBuffer();

	BuildLight();

	BuildBuffersAndViews();

	DoComputeWork();

	BuildOffscreenViews();

	BuildScreenGeometryBuffers();

	BuildQuadPatchBuffer();

	BuildTriPatchBuffers();

	return true;
}

bool D3D11RenderDevice::BuildTreeSpritesBuffer()
{
	Vertex::TreePointSprite v[TreeCount];

	for(UINT i = 0; i < TreeCount; ++i)
	{
		float x = MathHelper::RandF(-35.0f, 35.0f);
		float z = MathHelper::RandF(-35.0f, 35.0f);
		float y = 10.0f;//GetHillHeight(x,z);

		// Move tree slightly above land height.

		v[i].Pos  = XMFLOAT3(x,y,z);
		v[i].Size = XMFLOAT2(24.0f, 24.0f);
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::TreePointSprite) * TreeCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = v;
	HR(m_d3d11Device->CreateBuffer(&vbd, &vinitData, &m_TreeSpritesVB));

	Effects::InitAll();
	InputLayouts::InitAll(m_d3d11Device);
	RenderStates::InitAll(m_d3d11Device);

	return true;
}

bool D3D11RenderDevice::BuildCubeBuffer()
{
	// Create vertex buffer
	SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), (const float*)&Colors::White   },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), (const float*)&Colors::Black   },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), (const float*)&Colors::Red     },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), (const float*)&Colors::Green   },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), (const float*)&Colors::Blue    },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), (const float*)&Colors::Yellow  },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), (const float*)&Colors::Cyan    },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), (const float*)&Colors::Magenta }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(SimpleVertex) * 24;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	memset( &initData, 0, sizeof(initData));
	initData.pSysMem = vertices;

	HRESULT HR = m_d3d11Device->CreateBuffer(
		&vbd, &initData, &m_vertexBuff);

	if(FAILED(HR))
		return false;

	// Create indices 
	UINT indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3, 
		4, 3, 7
	};

	D3D11_BUFFER_DESC ibDesc;
	ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ibDesc.ByteWidth = sizeof(UINT) * 36;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initIndexData;
	initIndexData.pSysMem = indices;

	HR = m_d3d11Device->CreateBuffer( &ibDesc, &initIndexData, &m_indicesBuff);
	if(FAILED(HR))
		return false;

	return true;
}

bool D3D11RenderDevice::RenderCube()
{
	m_d3d11DeviceContext->IASetInputLayout(m_vertexDesc);
	m_d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = NULL;
	m_d3d11DeviceContext->IASetVertexBuffers( 0, 1, &m_vertexBuff, &stride, &offset);
	m_d3d11DeviceContext->IASetIndexBuffer( m_indicesBuff, DXGI_FORMAT_R32_UINT, 0);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_tech->GetPassByIndex(p)->Apply( 0, m_d3d11DeviceContext);

		m_d3d11DeviceContext->DrawIndexed( 36, 0, 0);
	}

	return true;
}

bool D3D11RenderDevice::RenderSpriteTree()
{
	m_treeEffect->SetDirLights(m_DirLights);
	m_treeEffect->SetEyePosW(m_EyePosW);
	m_treeEffect->SetFogColor(Colors::Silver);
	m_treeEffect->SetFogStart(15.0f);
	m_treeEffect->SetFogRange(175.0f);
	m_treeEffect->SetViewProj( m_View * m_Proj);
	m_treeEffect->SetMaterial( m_TreeMat);
	m_treeEffect->SetTreeTextureMapArray(m_TreeTextureMapArraySRV);

	m_d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	m_d3d11DeviceContext->IASetInputLayout(InputLayouts::TreePointSprite);
	UINT stride = sizeof(Vertex::TreePointSprite);
	UINT offset = 0;

	ID3DX11EffectTechnique* treeTech = m_treeEffect->Light3TexAlphaClipFogTech;
// 	switch(mRenderOptions)
// 	{
// 	case RenderOptions::Lighting:
// 		treeTech = m_treeEffect->Light3Tech;
// 		break;
// 	case RenderOptions::Textures:
// 		treeTech = m_treeEffect->Light3TexAlphaClipTech;
// 		break;
// 	case RenderOptions::TexturesAndFog:
// 		treeTech = m_treeEffect->Light3TexAlphaClipFogTech;
// 		break;
// 	}

	D3DX11_TECHNIQUE_DESC techDesc;
	treeTech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_d3d11DeviceContext->IASetVertexBuffers(0, 1, &m_TreeSpritesVB, &stride, &offset);

		float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};

		if(m_AlphaToCoverageOn)
		{
			m_d3d11DeviceContext->OMSetBlendState(RenderStates::AlphaToCoverageBS, blendFactor, 0xffffffff);
		}
		treeTech->GetPassByIndex(p)->Apply(0, m_d3d11DeviceContext);
		m_d3d11DeviceContext->Draw(TreeCount, 0);

		m_d3d11DeviceContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	}

	return true;
}

bool D3D11RenderDevice::BuildLight()
{
	m_DirLights[0].Ambient  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_DirLights[0].Diffuse  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_DirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_DirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	m_DirLights[1].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_DirLights[1].Diffuse  = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	m_DirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	m_DirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	m_DirLights[2].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_DirLights[2].Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_DirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_DirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

	m_TreeMat.Ambient  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_TreeMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_TreeMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

	return true;
}

void D3D11RenderDevice::DoComputeWork()
{
	D3DX11_TECHNIQUE_DESC techDesc;

	m_vecAddEffect->SetInputA(m_InputASRV);
	m_vecAddEffect->SetInputB(m_InputBSRV);
	m_vecAddEffect->SetOutput(m_OutputUAV);

	m_csTech->GetDesc(&techDesc);
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		ID3DX11EffectPass* pass = m_csTech->GetPassByIndex(p);
		pass->Apply(0, m_d3d11DeviceContext);

		m_d3d11DeviceContext->Dispatch(1, 1, 1);
	}

	// Unbind the input textures from the CS for good housekeeping.
	ID3D11ShaderResourceView* nullSRV[1] = { 0 };
	m_d3d11DeviceContext->CSSetShaderResources( 0, 1, nullSRV );

	// Unbind output from compute shader (we are going to use this output as an input in the next pass, 
	// and a resource cannot be both an output and input at the same time.
	ID3D11UnorderedAccessView* nullUAV[1] = { 0 };
	m_d3d11DeviceContext->CSSetUnorderedAccessViews( 0, 1, nullUAV, 0 );

	// Disable compute shader.
	m_d3d11DeviceContext->CSSetShader(0, 0, 0);

	std::ofstream fout("results.txt");
	// Copy the output buffer to system memory.
	m_d3d11DeviceContext->CopyResource(m_OutputDebugBuffer, m_OutputBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedData; 
	m_d3d11DeviceContext->Map(m_OutputDebugBuffer, 0, D3D11_MAP_READ, 0, &mappedData);

	Data* dataView = reinterpret_cast<Data*>(mappedData.pData);

	for(int i = 0; i < m_NumElements; ++i)
	{
		fout << "(" << dataView[i].v1.x << ", " << dataView[i].v1.y << ", " << dataView[i].v1.z <<
			", " << dataView[i].v2.x << ", " << dataView[i].v2.y << ")" << std::endl;
	}

	m_d3d11DeviceContext->Unmap(m_OutputDebugBuffer, 0);

	fout.close();
}

bool D3D11RenderDevice::BuildBuffersAndViews()
{
	std::vector<Data> dataA(m_NumElements);
	std::vector<Data> dataB(m_NumElements);
	for(int i = 0; i < m_NumElements; ++i)
	{
		dataA[i].v1 = XMFLOAT3(i, i, i);
		dataA[i].v2 = XMFLOAT2(i, 0);

		dataB[i].v1 = XMFLOAT3(-i, i, 0.0f);
		dataB[i].v2 = XMFLOAT2(0, -i);
	}

	// Create a buffer to be bound as a shader input (D3D11_BIND_SHADER_RESOURCE).
	D3D11_BUFFER_DESC inputDesc;
	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.ByteWidth = sizeof(Data) * m_NumElements;
	inputDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.StructureByteStride = sizeof(Data);
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	D3D11_SUBRESOURCE_DATA vinitDataA;
	vinitDataA.pSysMem = &dataA[0];

	ID3D11Buffer* bufferA = 0;
	HR(m_d3d11Device->CreateBuffer(&inputDesc, &vinitDataA, &bufferA));

	D3D11_SUBRESOURCE_DATA vinitDataB;
	vinitDataB.pSysMem = &dataB[0];

	ID3D11Buffer* bufferB = 0;
	HR(m_d3d11Device->CreateBuffer(&inputDesc, &vinitDataB, &bufferB));

	// Create a read-write buffer the compute shader can write to (D3D11_BIND_UNORDERED_ACCESS).
	D3D11_BUFFER_DESC outputDesc;
	outputDesc.Usage = D3D11_USAGE_DEFAULT;
	outputDesc.ByteWidth = sizeof(Data) * m_NumElements;
	outputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	outputDesc.CPUAccessFlags = 0;
	outputDesc.StructureByteStride = sizeof(Data);
	outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	HR(m_d3d11Device->CreateBuffer(&outputDesc, 0, &m_OutputBuffer));
	
	// Create a system memory version of the buffer to read the results back from.
	outputDesc.Usage = D3D11_USAGE_STAGING;
	outputDesc.BindFlags = 0;
	outputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	HR(m_d3d11Device->CreateBuffer(&outputDesc, 0, &m_OutputDebugBuffer));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = m_NumElements;

	m_d3d11Device->CreateShaderResourceView(bufferA, &srvDesc, &m_InputASRV);
	m_d3d11Device->CreateShaderResourceView(bufferB, &srvDesc, &m_InputBSRV);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Buffer.NumElements = m_NumElements;

	m_d3d11Device->CreateUnorderedAccessView(m_OutputBuffer, &uavDesc, &m_OutputUAV);

	// Views hold references to buffers, so we can release these.
	SAFE_RELEASE(bufferA);
	SAFE_RELEASE(bufferB);


	return true;
}

bool D3D11RenderDevice::RenderScreen()
{
	m_d3d11DeviceContext->IASetInputLayout(InputLayouts::Basic32);
	m_d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	XMMATRIX identity = XMMatrixIdentity();

	ID3DX11EffectTechnique* texOnlyTech = m_basicEffect->Light0TexTech;
	D3DX11_TECHNIQUE_DESC techDesc;

	texOnlyTech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_d3d11DeviceContext->IASetVertexBuffers(0, 1, &m_ScreenQuadVB, &stride, &offset);
		m_d3d11DeviceContext->IASetIndexBuffer(m_ScreenQuadIB, DXGI_FORMAT_R32_UINT, 0);

		m_basicEffect->SetWorld(identity);
		m_basicEffect->SetWorldInvTranspose(identity);
		m_basicEffect->SetWorldViewProj(identity);
		m_basicEffect->SetTexTransform(identity);
		m_basicEffect->SetDiffuseMap(m_blurFilter->GetBlurredOutput());

		texOnlyTech->GetPassByIndex(p)->Apply(0, m_d3d11DeviceContext);
		m_d3d11DeviceContext->DrawIndexed(6, 0, 0);
	}

	return true;
}

bool D3D11RenderDevice::BuildOffscreenViews()
{
	// We call this function everytime the window is resized so that the render target is a quarter
	// the client area dimensions.  So Release the previous views before we create new ones.
	SAFE_RELEASE(m_OffscreenSRV);
	SAFE_RELEASE(m_OffscreenRTV);
	SAFE_RELEASE(m_OffscreenUAV);

	D3D11_TEXTURE2D_DESC texDesc;

	texDesc.Width     = 640;
	texDesc.Height    = 480;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format    = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count   = 1;  
	texDesc.SampleDesc.Quality = 0;  
	texDesc.Usage          = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags      = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0; 
	texDesc.MiscFlags      = 0;

	ID3D11Texture2D* offscreenTex = 0;
	HR(m_d3d11Device->CreateTexture2D(&texDesc, 0, &offscreenTex));

	// Null description means to create a view to all mipmap levels using 
	// the format the texture was created with.
	HR(m_d3d11Device->CreateShaderResourceView(offscreenTex, 0, &m_OffscreenSRV));
	HR(m_d3d11Device->CreateRenderTargetView(offscreenTex, 0, &m_OffscreenRTV));
	HR(m_d3d11Device->CreateUnorderedAccessView(offscreenTex, 0, &m_OffscreenUAV));

	// View saves a reference to the texture so we can release our reference.
	SAFE_RELEASE(offscreenTex);

	return true;
}

bool D3D11RenderDevice::BuildScreenGeometryBuffers()
{
	GeometryGenerator::MeshData quad;

	GeometryGenerator geoGen;
	geoGen.CreateFullscreenQuad(quad);

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	std::vector<Vertex::Basic32> vertices(quad.Vertices.size());

	for(UINT i = 0; i < quad.Vertices.size(); ++i)
	{
		vertices[i].Pos    = quad.Vertices[i].Position;
		vertices[i].Normal = quad.Vertices[i].Normal;
		vertices[i].Tex    = quad.Vertices[i].TexC;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic32) * quad.Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_d3d11Device->CreateBuffer(&vbd, &vinitData, &m_ScreenQuadVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * quad.Indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &quad.Indices[0];
	HR(m_d3d11Device->CreateBuffer(&ibd, &iinitData, &m_ScreenQuadIB));

	return true;
}

bool D3D11RenderDevice::RenderTessellation()
{
	m_d3d11DeviceContext->IASetInputLayout(InputLayouts::Pos);
	m_d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	UINT stride = sizeof(Vertex::Pos);
	UINT offset = 0;

	// Set per frame constants.
	m_tsEffect->SetEyePosW(m_EyePosW);
	m_tsEffect->SetFogColor(Colors::Silver);
	m_tsEffect->SetFogStart(15.0f);
	m_tsEffect->SetFogRange(175.0f);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_tsEffect->TessTech->GetDesc( &techDesc );

	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_d3d11DeviceContext->IASetVertexBuffers(0, 1, &m_QuadPatchVB, &stride, &offset);

		// Set per object constants.
		XMMATRIX world = XMMatrixIdentity();
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*m_View*m_Proj;

		m_tsEffect->SetWorld(world);
		m_tsEffect->SetWorldInvTranspose(worldInvTranspose);
		m_tsEffect->SetWorldViewProj(worldViewProj);
		m_tsEffect->SetTexTransform(XMMatrixIdentity());
		//m_tsEffect->SetMaterial(0);
		m_tsEffect->SetDiffuseMap(0);

		m_tsEffect->TessTech->GetPassByIndex(p)->Apply(0, m_d3d11DeviceContext);

		//m_d3d11DeviceContext->RSSetState(RenderStates::WireframeRS);
		m_d3d11DeviceContext->Draw(4, 0);
	}

	return true;
}

bool D3D11RenderDevice::BuildQuadPatchBuffer()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	XMFLOAT3 vertices[4] = 
	{
		XMFLOAT3(-10.0f, 0.0f, +10.0f),
		XMFLOAT3(+10.0f, 0.0f, +10.0f),
		XMFLOAT3(-10.0f, 0.0f, -10.0f),
		XMFLOAT3(+10.0f, 0.0f, -10.0f)
	};

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_d3d11Device->CreateBuffer(&vbd, &vinitData, &m_QuadPatchVB));

	return true;
}

bool D3D11RenderDevice::BuildTriPatchBuffers()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) * 3;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	XMFLOAT3 vertices[3] = 
	{
		XMFLOAT3(-10.0f, 0.0f, -10.0f),
		XMFLOAT3(  0.0f, 0.0f, +10.0f),
		XMFLOAT3(+10.0f, 0.0f, -10.0f),
	};

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_d3d11Device->CreateBuffer(&vbd, &vinitData, &m_TriPatchVB));

	return true;
}

bool D3D11RenderDevice::RenderTriangleTessellation()
{
	m_d3d11DeviceContext->IASetInputLayout(InputLayouts::Pos);
	m_d3d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	UINT stride = sizeof(Vertex::Pos);
	UINT offset = 0;

	// Set per frame constants.
	m_triEffect->SetEyePosW(m_EyePosW);
	m_triEffect->SetFogColor(Colors::Silver);
	m_triEffect->SetFogStart(15.0f);
	m_triEffect->SetFogRange(175.0f);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_triEffect->TessTech->GetDesc( &techDesc );

	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_d3d11DeviceContext->IASetVertexBuffers(0, 1, &m_TriPatchVB, &stride, &offset);

		// Set per object constants.
		XMMATRIX world = XMMatrixIdentity();
		XMMATRIX offMatrix =  XMMatrixTranslation( 10.0f, 10.0f, 0.0f);
		world *= offMatrix;
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*m_View*m_Proj;

		m_triEffect->SetWorld(world);
		m_triEffect->SetWorldInvTranspose(worldInvTranspose);
		m_triEffect->SetWorldViewProj(worldViewProj);
		m_triEffect->SetTexTransform(XMMatrixIdentity());
		//m_triEffect->SetMaterial(0);
		m_triEffect->SetDiffuseMap(0);

		m_triEffect->TessTech->GetPassByIndex(p)->Apply(0, m_d3d11DeviceContext);

		m_d3d11DeviceContext->RSSetState(RenderStates::WireframeRS);
		m_d3d11DeviceContext->Draw(3, 0);
	}

	return true;
}


D3D11RenderDevice D3D11RenderDevice::m_Instance;

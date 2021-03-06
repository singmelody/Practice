#include "StdAfx.h"
#include "EngineConfig.h"
#include "D3D9Renderer.h"
#include "D3D9RenderDevice.h"
#include "D3D9IndexBuffer.h"
#include "D3D9VertexBuffer.h"
#include "D3D9VertexLayout.h"

// Material Header
#include "D3D9Shader.h"
#include "Material/GlobalShader.h"
#include "Math/Matrix.h"
#include "IMaterial.hpp"

#include "IEngine.hpp"
#include "IMaterialManager.hpp"
#include <assert.h>

namespace Dream{

//---- temp code -----
struct CUSTOMVERTEX
{
	FLOAT x, y, z; // The transformed position for the vertex
	DWORD color;        // The vertex color
};


//--------------------

D3D9Renderer::D3D9Renderer() : IRenderer()
	,m_ib(NULL)
	,m_vb(NULL)
	,m_curMaterial(NULL)
{
	m_device = new D3D9RenderDevice();
	m_globalShader = new GlobalShader();
}


D3D9Renderer::~D3D9Renderer()
{
	SAFE_DELETE(m_globalShader);
	SAFE_DELETE(m_device);
}

bool D3D9Renderer::Init(const void* wnd, IEngine* engine)
{
	HWND hwnd = (HWND)wnd;
	if(!hwnd)
		return false;

	gEngine = engine;
	if(!gEngine)
		return false;
	
	// device init
	m_device->Init(wnd);

	// Step 1: check hardware caps to create property device
	bool result = false; 
	result = m_device->CheckCaps();
	if(!result)
		return false;

	// Step 2: Create Device
	result = m_device->CreateDevice();
	if(!result)
		return false;

	InitCube();

	return true;
}

void D3D9Renderer::Destroy()
{
	SAFE_DELETE(m_ib);
	SAFE_DELETE(m_vb);

	SAFE_DELETE(m_device);
}

#include <mmsystem.h>
void D3D9Renderer::Update(float deltaTime)
{
	D3DXMATRIXA16 matWorld;

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * ( 2.0f * D3DX_PI ) / 1000.0f;
	D3DXMatrixRotationY( &matWorld, fAngle );

 	IMaterial* material = gEngine->GetMaterialManager()->CreateMaterial("simple.mat");
 	material->Load();
 
 	SetMaterial(material);


	D3DXVECTOR3 vEyePt( 0.0f, 3.0f,-5.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	Matrix matView;
	//D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );

	m_globalShader->SetShaderParam( GlobalShader::eMatView, matView);

	Matrix matProj;
	//D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 800.0f/600.0f, 1.0f, 100.0f );
	m_globalShader->SetShaderParam( GlobalShader::eMatProj, matProj);
}

void D3D9Renderer::Render()
{
	m_device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0), 1.0f, 0);

	m_device->BeginScene();
	
	// set declaration & vertexbuffer
	m_device->SetVertexBuffer( 0, m_vb, 0);
	
	m_device->SetIndexBuffer( m_ib );

	SetMaterial(m_curMaterial);

	m_device->EndScene();

	m_device->Present();
}

//-- temp code---
void D3D9Renderer::InitCube()
{
// 	// Turn off culling, so we see the front and back of the triangle
 	m_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
// 
// 	// Turn off D3D lighting, since we are providing our own vertex colors
 	m_device->SetRenderState( D3DRS_LIGHTING, FALSE );

	// Initialize three Vertices for rendering a triangle
	CUSTOMVERTEX pVertex[] =
	{
		{ -1.0f, 1.0f, -1.0f, 0x0000ffff, },
		{  1.0f, 1.0f, -1.0f, 0x00ff00ff, },
		{  1.0f, 1.0f, 1.0f,  0x00ffffff, },
		{ -1.0f, 1.0f, 1.0f,  0xff0000ff, },
		{ -1.0f,-1.0f,-1.0f,  0xff00ffff, },
		{  1.0f,-1.0f,-1.0f,  0xffff00ff, },
		{  1.0f, -1.0f, 1.0f, 0xffffffff, },
		{ -1.0f, -1.0f, 1.0f, 0x000000ff, },
	};

	IVertexLayout* layout = new D3D9VertexLayout();
	layout->AddSignature( 0, 0, IVertexLayout::FLOAT3, IVertexLayout::DEFAULT, IVertexLayout::POSITION, 0);
	layout->AddSignature( 0, 12, IVertexLayout::DWORD, IVertexLayout::DEFAULT, IVertexLayout::COLOR, 0);
	layout->Build(m_device);

	IDirect3DVertexBuffer9* vb;
	if( FAILED(m_device->CreateVertexBuffer( sizeof( pVertex ),
		NULL, NULL,
		D3DPOOL_DEFAULT, &vb, NULL )) )
		return;


	m_vb = new D3D9VertexBuffer(vb);
	if(!m_vb)
		return;

	m_vb->SetIVertexLayout(layout);

	m_vb->Fill((void*)pVertex, sizeof(pVertex));


	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};

	IDirect3DIndexBuffer9* ib;
	bool result = m_device->CreateIndexBuffer(
		sizeof(indices), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &ib, NULL);

	if(!result)
		return;

	m_ib = new D3D9IndexBuffer(ib);
	if(!m_ib)
		return;

	
	m_ib->Fill(indices, sizeof(indices));

	return;
}

IRenderDevice* D3D9Renderer::GetIRenderDevice()
{
	return m_device;
}

void D3D9Renderer::SetMaterial(IMaterial* material)
{
	m_curMaterial = material;
}

void D3D9Renderer::RenderCallBack()
{
	m_device->Draw( m_firstVertex, m_vertexCount, m_firstIdx, m_idxCount);
}

//--------------------------------------------------------------------
/// Dll Exports
//--------------------------------------------------------------------
extern "C" DLL_EXPORT void CreateRenderer(IRenderer** oRenderer)
{
	static D3D9Renderer  d3d9Renderer;
	*oRenderer = &d3d9Renderer;
}

}

#include "DXUT.h"
#include "Face.h"
#include "BoneHierarchyLoader.h"
#include "Utils.h"

Face::Face(void)
{
	m_faceMesh = NULL;
	m_faceTexture = NULL;
}

Face::~Face(void)
{
	SAFE_RELEASE(m_faceMesh);
	SAFE_RELEASE(m_faceTexture);
}

void Face::Init()
{
	// Load Mesh
	LoadMesh( "meshes\\face.x", &m_faceMesh);

	// Load Texture
	LoadTex( "meshes\\face.jpg", &m_faceTexture);

	//Init Eyes
	m_eyes[0].Init(D3DXVECTOR3(-0.037f, 0.04f, -0.057f));
	m_eyes[1].Init(D3DXVECTOR3( 0.037f, 0.04f, -0.057f));
}

void Face::Update(float deltaTime)
{
	POINT p;
	GetCursorPos(&p);

	float x = ( p.x - 320.0f) / 640.0f;
	float y = ( p.y - 240.0f) / 480.0f;

	// tell both eyes to look at point
	m_eyes[0].LookAt(D3DXVECTOR3(x,y,-1.0f));
	m_eyes[1].LookAt(D3DXVECTOR3(x,y,-1.0f));
}

void Face::Render(const char* tech)
{
	g_pEffect->SetTexture("g_MeshTexture", m_faceTexture);

	//Start Technique
	D3DXHANDLE hTech = g_pEffect->GetTechniqueByName(tech);
	g_pEffect->SetTechnique(hTech);
	g_pEffect->Begin(NULL, NULL);
	g_pEffect->BeginPass(0);

	//Render Face
	m_faceMesh->DrawSubset(0);

	//Render Eyes
	m_eyes[0].Render();
	m_eyes[1].Render();

	g_pEffect->EndPass();
	g_pEffect->End();
}

ComplexFace::ComplexFace(const std::string& filename)
{
	m_baseMesh = NULL;
	m_binkMesh = NULL;
	m_FaceTexture = NULL;
	m_pFaceVertexDecl = NULL;


	//Face Vertex Format
	D3DVERTEXELEMENT9 faceVertexDecl[] = 
	{
		//1st Stream: Base Mesh
		{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},	
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},	

		//2nd Stream
		{1,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 1},
		{1, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   1},
		{1, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},

		//3rd Stream
		{2,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 2},
		{2, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   2},
		{2, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},

		//4th Stream
		{3,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 3},
		{3, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   3},
		{3, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},

		//5th Stream
		{4,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 4},
		{4, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   4},
		{4, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},

		D3DDECL_END()
	};

	// Create face vertex decl
	DXUTGetD3D9Device()->CreateVertexDeclaration( faceVertexDecl, &m_pFaceVertexDecl);

	// Load Face Meshes
	FaceHierarchyLoader hierarchy;
	D3DXFRAME*	root = NULL;
	LoadMeshHierarchy( filename.c_str(), &hierarchy, &root);

	//Extract Face Meshes
	ExtractMeshes(root);

	//Destroy temporary hierarchy
	hierarchy.DestroyFrame(root);

	//Load texture
	LoadTex( "meshes\\face.jpg", &m_FaceTexture);

	//Init Eyes
	m_eyes[0].Init(D3DXVECTOR3(-0.037f, 0.04f, -0.057f));
	m_eyes[1].Init(D3DXVECTOR3( 0.037f, 0.04f, -0.057f));
}

ComplexFace::~ComplexFace()
{
	SAFE_RELEASE( m_baseMesh );
	SAFE_RELEASE( m_binkMesh );
	SAFE_RELEASE( m_FaceTexture );
	SAFE_RELEASE( m_pFaceVertexDecl );

	for (int i = 0; i < m_emotionMeshes.size(); ++i)
	{
		SAFE_RELEASE(m_emotionMeshes[i]);
	}

	for (int i = 0; i < m_speechMeshes.size(); ++i)
	{
		SAFE_RELEASE(m_speechMeshes[i]);
	}
}

void ComplexFace::Update(float deltaTime)
{
	float time = GetTickCount() * 0.002f;

	m_morphWeights = D3DXVECTOR4( sin(time * 1.0f),
								  cos(time * 0.9f),
								  sin(time * 0.7f),
								  cos(time * 1.2f));

	POINT p;
	GetCursorPos(&p);

	float x = ( p.x - 320.0f) / 640.0f;
	float y = ( p.y - 240.0f) / 480.0f;

	// tell both eyes to look at point
	m_eyes[0].LookAt(D3DXVECTOR3(x,y,-1.0f));
	m_eyes[1].LookAt(D3DXVECTOR3(x,y,-1.0f));
}

void ComplexFace::Render(const char* tech)
{
	if(!m_baseMesh)
		return;

	// set active vertex decl
	DXUTGetD3D9Device()->SetVertexDeclaration(m_pFaceVertexDecl);

	// Set Streams
	DWORD vSize1 = m_baseMesh->GetNumBytesPerVertex();
	DWORD vSize = D3DXGetFVFVertexSize( m_baseMesh->GetFVF()); 
	assert( vSize1 == vSize );

	IDirect3DVertexBuffer9* baseMeshBuffer = NULL;
	m_baseMesh->GetVertexBuffer(&baseMeshBuffer);
	DXUTGetD3D9Device()->SetStreamSource( 0, baseMeshBuffer, 0 , vSize);
	SAFE_RELEASE(baseMeshBuffer);

	// Temp : Set some random source meshes
	for (int i = 0; i < 4; ++i)
	{
		IDirect3DVertexBuffer9* meshBuffer = NULL;

		if (m_morphWeights[i] >= 0.0f)
		{
			m_emotionMeshes[i]->GetVertexBuffer(&meshBuffer);
		}
		else
		{
			m_speechMeshes[i]->GetVertexBuffer(&meshBuffer);
			m_morphWeights[i] *= -1.0f;
		}
		DXUTGetD3D9Device()->SetStreamSource( i + 1, meshBuffer, 0, vSize);

		SAFE_RELEASE(meshBuffer);
	}

	// Set Index buffer
	IDirect3DIndexBuffer9* ib = NULL;
	m_baseMesh->GetIndexBuffer(&ib);
	DXUTGetD3D9Device()->SetIndices(ib);
	SAFE_RELEASE(ib);

	// Set Shader var
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	g_pEffect->SetMatrix("g_mWorld", &world);
	g_pEffect->SetTexture("g_MeshTexture", m_FaceTexture);
	g_pEffect->SetVector("weights", &m_morphWeights);

	//Start Technique
	D3DXHANDLE hTech = g_pEffect->GetTechniqueByName("MultiMorph");
	g_pEffect->SetTechnique(hTech);
	g_pEffect->Begin(NULL, NULL);
	g_pEffect->BeginPass(0);

	//Draw mesh
	DXUTGetD3D9Device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 
		m_baseMesh->GetNumVertices(), 0, 
		m_baseMesh->GetNumFaces());

	g_pEffect->EndPass();
	g_pEffect->End();

	//Restore vertex declaration and stream sources
	DXUTGetD3D9Device()->SetVertexDeclaration(NULL);

	for(int i=0;i<5;i++)
	{
		DXUTGetD3D9Device()->SetStreamSource(i, NULL, 0, 0);
	}

	//Render Eyes
	hTech = g_pEffect->GetTechniqueByName(tech);
	g_pEffect->SetTechnique(hTech);
	g_pEffect->Begin(NULL, NULL);
	g_pEffect->BeginPass(0);

	m_eyes[0].Render();
	m_eyes[1].Render();

	g_pEffect->EndPass();
	g_pEffect->End();

}

void ComplexFace::ExtractMeshes(D3DXFRAME* frame)
{
	if (frame == NULL)
		return;

	if(frame->pMeshContainer)
	{
		if(frame->Name)
		{
			ID3DXMesh* mesh = (ID3DXMesh*)frame->pMeshContainer;

			if(!strcmp(frame->Name, "Base"))
			{
				m_baseMesh = mesh;
				mesh->AddRef();
			}
			else if(!strcmp(frame->Name, "Blink") )
			{
				m_binkMesh = mesh;
				mesh->AddRef();
			}
			else if(!strcmp(frame->Name, "Emotion") )
			{
				m_emotionMeshes.push_back(mesh);
				mesh->AddRef();
			}
			else if(!strcmp(frame->Name, "Speech") )
			{
				m_speechMeshes.push_back(mesh);
				mesh->AddRef();
			}
		}
	}

	ExtractMeshes(frame->pFrameFirstChild);
	ExtractMeshes(frame->pFrameSibling);
}

#include "DXUT.h"
#include "Face.h"
#include "BoneHierarchyLoader.h"
#include "Utils.h"
#include "FaceController.h"
#include <iostream>

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

	//Render Eyes
	hTech = g_pEffect->GetTechniqueByName(tech);
	g_pEffect->SetTechnique(hTech);
	g_pEffect->Begin(NULL, NULL);
	g_pEffect->BeginPass(0);

	m_eyes[0].Render();
	m_eyes[1].Render();

	g_pEffect->EndPass();
	g_pEffect->End();

	//Restore vertex declaration and stream sources
	DXUTGetD3D9Device()->SetVertexDeclaration(NULL);

	for(int i=0;i<5;i++)
	{
		DXUTGetD3D9Device()->SetStreamSource(i, NULL, 0, 0);
	}

}

void ComplexFace::ExtractMeshes(D3DXFRAME* frame)
{
	if (frame == NULL)
		return;

	if(frame->pMeshContainer)
	{
		if(frame->Name)
		{
			ID3DXMesh* mesh = (ID3DXMesh*)frame->pMeshContainer->MeshData.pMesh;

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

FaceModel::FaceModel(const char* filename)
{
	m_baseMesh = NULL;
	m_binkMesh = NULL;
	m_faceTex = NULL;
	m_normalTex = NULL;
	m_pFaceVertexDecl = NULL;

	//Face Vertex Format
	//Face Vertex Format
	D3DVERTEXELEMENT9 faceVertexDecl[] = 
	{
		//1st Stream: Base Mesh
		{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},	
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},	
		{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0},
		{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},

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
	LoadMeshHierarchy( filename, &hierarchy, &root);

	//Extract Face Meshes
	ExtractMeshes(root);

	// Add tagents & binormal to base mesh
	AddTangent(&m_baseMesh);
	//PrintMeshDeclaration(m_pBaseMesh);

	//Destroy temporary hierarchy
	hierarchy.DestroyFrame(root);

	//Load texture
	LoadTex( "meshes\\face.jpg", &m_faceTex);
	LoadTex( "meshes\\face_normal.tga", &m_normalTex);
}

FaceModel::FaceModel()
{
	m_baseMesh = NULL;
	m_binkMesh = NULL;
	m_faceTex = NULL;
	m_normalTex = NULL;
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
}


FaceModel::~FaceModel()
{
	SAFE_RELEASE( m_baseMesh );
	SAFE_RELEASE( m_binkMesh );
	SAFE_RELEASE( m_faceTex );
	SAFE_RELEASE( m_pFaceVertexDecl );
	SAFE_RELEASE( m_normalTex );

	for (int i = 0; i < m_emotionMeshes.size(); ++i)
	{
		SAFE_RELEASE(m_emotionMeshes[i]);
	}

	for (int i = 0; i < m_speechMeshes.size(); ++i)
	{
		SAFE_RELEASE(m_speechMeshes[i]);
	}
}

void FaceModel::Render(FaceController* pController)
{
	if( m_baseMesh == NULL || pController == NULL)
		return;

	// set active vertex decl
	DXUTGetD3D9Device()->SetVertexDeclaration(m_pFaceVertexDecl);

	// set streams
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	m_baseMesh->GetDeclaration(decl);
	DWORD vSize1 = D3DXGetDeclVertexSize(decl, 0);
	IDirect3DVertexBuffer9* baseMeshBuffer = NULL;
	m_baseMesh->GetVertexBuffer(&baseMeshBuffer);
	DXUTGetD3D9Device()->SetStreamSource(0, baseMeshBuffer, 0, vSize1);
	SAFE_RELEASE(baseMeshBuffer);

	// set blink source
	DWORD vSize = D3DXGetFVFVertexSize(m_binkMesh->GetFVF());
	IDirect3DVertexBuffer9* blinkBuffer = NULL;
	m_binkMesh->GetVertexBuffer(&blinkBuffer);
	DXUTGetD3D9Device()->SetStreamSource( 1, blinkBuffer, 0, vSize);
	SAFE_RELEASE(blinkBuffer);

	// set emotion source
	IDirect3DVertexBuffer9* emotionBuffer = NULL;
	m_emotionMeshes[pController->m_emotionIdx]->GetVertexBuffer(&emotionBuffer);
	DXUTGetD3D9Device()->SetStreamSource( 2, emotionBuffer, 0, vSize);
	SAFE_RELEASE(emotionBuffer);

	// set speech source
	for (int i = 0; i < 2; ++i)
	{
		IDirect3DVertexBuffer9* speechBuffer = NULL;
		m_speechMeshes[pController->m_speechIndices[i]]->GetVertexBuffer(&speechBuffer);
		DXUTGetD3D9Device()->SetStreamSource( 3 + i, speechBuffer, 0, vSize);
		SAFE_RELEASE(speechBuffer);
	}

	//Set Index buffer
	IDirect3DIndexBuffer9* ib = NULL;
	m_baseMesh->GetIndexBuffer(&ib);
	DXUTGetD3D9Device()->SetIndices(ib);
	SAFE_RELEASE(ib);

	// Set Shader var
	g_pEffect->SetMatrix("g_mWorld", &pController->m_headMatrix);
	g_pEffect->SetTexture("texDiffuse", m_faceTex);
	g_pEffect->SetTexture("texNormalMap", m_normalTex);
	g_pEffect->SetVector("weights", &pController->m_morphWeights);

	//Start Technique
	D3DXHANDLE hTech = g_pEffect->GetTechniqueByName("FaceMorphNormal");
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
}

void FaceModel::ExtractMeshes(D3DXFRAME* frame)
{
	if (frame == NULL)
		return;

	if(frame->pMeshContainer)
	{
		if(frame->Name)
		{
			ID3DXMesh* mesh = (ID3DXMesh*)frame->pMeshContainer->MeshData.pMesh;

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

void FaceModel::AddTangent(ID3DXMesh** pMesh)
{
	//Get vertex declaration from mesh
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	(*pMesh)->GetDeclaration(decl);

	//Find the end index of the declaration
	int index = 0;
	while(decl[index].Type != D3DDECLTYPE_UNUSED)
	{
		index++;
	}

	//Get size of last element
	int size = 0;

	switch(decl[index - 1].Type)
	{
	case D3DDECLTYPE_FLOAT1:
		size = 4;
		break;

	case D3DDECLTYPE_FLOAT2:
		size = 8;
		break;

	case D3DDECLTYPE_FLOAT3:
		size = 12;
		break;

	case D3DDECLTYPE_FLOAT4:
		size = 16;
		break;

	case D3DDECLTYPE_D3DCOLOR:
		size = 4;
		break;

	case D3DDECLTYPE_UBYTE4:
		size = 4;
		break;

	default:
		std::cout << "Unhandled declaration type: " << decl[index - 1].Type << "\n";
	};

	//Create Tangent Element
	D3DVERTEXELEMENT9 tangent = 
	{
		0, 
		decl[index - 1].Offset + size,
		D3DDECLTYPE_FLOAT3, 
		D3DDECLMETHOD_DEFAULT, 
		D3DDECLUSAGE_TANGENT, 
		0
	};

	//Create BiNormal Element
	D3DVERTEXELEMENT9 binormal = 
	{
		0, 
		tangent.Offset + 12, 
		D3DDECLTYPE_FLOAT3, 
		D3DDECLMETHOD_DEFAULT, 
		D3DDECLUSAGE_BINORMAL, 
		0
	};

	//End element
	D3DVERTEXELEMENT9 endElement = D3DDECL_END();

	//Add new elements to vertex declaration
	decl[index++] = tangent;
	decl[index++] = binormal;
	decl[index] = endElement;

	//Convert mesh to the new vertex declaration
	ID3DXMesh* pNewMesh = NULL;

	if(FAILED((*pMesh)->CloneMesh((*pMesh)->GetOptions(), 
		decl,
		DXUTGetD3D9Device(),
		&pNewMesh)))
	{
		std::cout << "Failed to clone mesh\n";
		return;
	}

	//Compute the tangents & binormals
	if(FAILED(D3DXComputeTangentFrame(pNewMesh, NULL)))
	{
		SAFE_RELEASE( pNewMesh );
		std::cout << "Failed to compute tangents & binormals for new mesh\n";
		return;
	}

	//Release old mesh
	SAFE_RELEASE( *pMesh );

	//Assign new mesh to the mesh pointer
	*pMesh = pNewMesh;
}

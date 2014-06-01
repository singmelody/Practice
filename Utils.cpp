#include "DXUT.h"
#include "SDKmisc.h"
#include "Utils.h"
#include <string>
#include "Morph.h"
#include <iostream>
#include <D3DX11async.h>
#include "D3D11RenderDevice.h"

std::wstring GetWC(const char *c)
{
	const size_t cSize = strlen(c)+1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs (wc, c, cSize);

	std::wstring wstr;
	wstr = wc;
	SAFE_DELETE(wc);

	return wstr;
}

void LoadMesh(const char* c, ID3DXMesh** mesh, ID3DXBuffer** ppAdjacency, ID3DXBuffer** ppMaterials, DWORD* materialCount)
{
	std::wstring conv = GetWC(c);

	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str() );
	D3DXLoadMeshFromX(str, D3DXMESH_MANAGED, DXUTGetD3D9Device(), ppAdjacency, ppMaterials, NULL, materialCount, mesh);
}

void LoadTex(const char* c, IDirect3DTexture9** tex)
{
	std::wstring conv = GetWC(c);

	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str() );
	D3DXCreateTextureFromFile(DXUTGetD3D9Device(), str, tex);
}

extern bool LoadShader(const char* c, ID3DX11Effect* &effect)
{
	std::wstring conv = GetWC(c);

	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str());
	
	DWORD compileFlag = NULL;

#if _DEBUG
	compileFlag |= D3D10_SHADER_DEBUG;
	compileFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	ID3D10Blob* compiledShader = NULL;
	ID3D10Blob* compiledError = NULL;
	HRESULT hr = D3DX11CompileFromFile( str, NULL, NULL, NULL, "fx_5_0", compileFlag, NULL, NULL, &compiledShader, &compiledError, NULL);

	if( compiledError != NULL || FAILED(hr))
	{
		SAFE_RELEASE(compiledError);
		return false;
	}

	ID3D11Device* device = D3D11RenderDevice::Instance().m_d3d11Device;
	hr = D3DX11CreateEffectFromMemory( compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
		0, device, &effect);
	if(hr == D3D11_ERROR_FILE_NOT_FOUND)
	{
		return false;
	}
	else if(hr == D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS)
	{
		return false;
	}
	else if(hr == D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS)
	{
		return false;
	}
	else if(hr == D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD)
	{
		return false;
	}

	if(FAILED(hr))
		return false;

	SAFE_RELEASE(compiledShader);
	SAFE_RELEASE(compiledError);

	return true;
}


extern void LoadMeshHierarchy(const char* c, const LPD3DXALLOCATEHIERARCHY& hier, D3DXFRAME** frame)
{
	std::wstring conv = GetWC(c);

	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str() );
	D3DXLoadMeshHierarchyFromX(str, D3DXMESH_MANAGED, DXUTGetD3D9Device(), hier, NULL, frame, NULL);
}

void PrintMeshDeclaration(ID3DXMesh* pMesh)
{
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	pMesh->GetDeclaration(decl);

	for(int i=0; i<MAX_FVF_DECL_SIZE; i++)
	{
		if(decl[i].Type != D3DDECLTYPE_UNUSED)
		{
			std::cout << "Offset: " << (int)decl[i].Offset << ", Type: " << (int)decl[i].Type << ", Usage: " << (int)decl[i].Usage << "\n";
		}
		else break;
	}
}

extern ID3D11ShaderResourceView* CreateTexture2DArraySRV(ID3D11Device* device, ID3D11DeviceContext* context, std::vector<std::string>& filenames, DXGI_FORMAT format /*= DXGI_FORMAT_FROM_FILE*/, UINT filter /*= D3DX11_FILTER_NONE*/, UINT mipFilter /*= D3DX11_FILTER_LINEAR*/)
{
	//
	// Load the texture elements individually from file.  These textures
	// won't be used by the GPU (0 bind flags), they are just used to 
	// load the image data from file.  We use the STAGING usage so the
	// CPU can read the resource.
	//

	UINT size = filenames.size();


	std::vector<std::wstring> finalFileNames;
	for (size_t i = 0; i < size; ++i)
	{
		std::wstring conv = GetWC(filenames[i].c_str());

		WCHAR str[MAX_PATH];
		DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv.c_str() );

		finalFileNames.push_back(str);
	}


	std::vector<ID3D11Texture2D*> srcTex(size);
	for(UINT i = 0; i < size; ++i)
	{
		D3DX11_IMAGE_LOAD_INFO loadInfo;

		loadInfo.Width  = D3DX11_FROM_FILE;
		loadInfo.Height = D3DX11_FROM_FILE;
		loadInfo.Depth  = D3DX11_FROM_FILE;
		loadInfo.FirstMipLevel = 0;
		loadInfo.MipLevels = D3DX11_FROM_FILE;
		loadInfo.Usage = D3D11_USAGE_STAGING;
		loadInfo.BindFlags = 0;
		loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		loadInfo.MiscFlags = 0;
		loadInfo.Format = format;
		loadInfo.Filter = filter;
		loadInfo.MipFilter = mipFilter;
		loadInfo.pSrcInfo  = 0;

		HR(D3DX11CreateTextureFromFile(device, finalFileNames[i].c_str(), 
			&loadInfo, 0, (ID3D11Resource**)&srcTex[i], 0));
	}

	//
	// Create the texture array.  Each element in the texture 
	// array has the same format/dimensions.
	//

	D3D11_TEXTURE2D_DESC texElementDesc;
	srcTex[0]->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width              = texElementDesc.Width;
	texArrayDesc.Height             = texElementDesc.Height;
	texArrayDesc.MipLevels          = texElementDesc.MipLevels;
	texArrayDesc.ArraySize          = size;
	texArrayDesc.Format             = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count   = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage              = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags     = 0;
	texArrayDesc.MiscFlags          = 0;

	ID3D11Texture2D* texArray = 0;
	HR(device->CreateTexture2D( &texArrayDesc, 0, &texArray));

	//
	// Copy individual texture elements into texture array.
	//

	// for each texture element...
	for(UINT texElement = 0; texElement < size; ++texElement)
	{
		// for each mipmap level...
		for(UINT mipLevel = 0; mipLevel < texElementDesc.MipLevels; ++mipLevel)
		{
			D3D11_MAPPED_SUBRESOURCE mappedTex2D;
			HR(context->Map(srcTex[texElement], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D));

			context->UpdateSubresource(texArray, 
				D3D11CalcSubresource(mipLevel, texElement, texElementDesc.MipLevels),
				0, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);

			context->Unmap(srcTex[texElement], mipLevel);
		}
	}	

	//
	// Create a resource view to the texture array.
	//

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = size;

	ID3D11ShaderResourceView* texArraySRV = 0;
	HR(device->CreateShaderResourceView(texArray, &viewDesc, &texArraySRV));

	//
	// Cleanup--we only need the resource view.
	//

	SAFE_RELEASE(texArray);

	for(UINT i = 0; i < size; ++i)
		SAFE_RELEASE(srcTex[i]);

	return texArraySRV;
}

ID3DXEffect*    g_pEffect = NULL;       // D3DX effect interface
SkinnedMesh*	g_SkinnedMesh = NULL;
ID3DXLine*		g_Line = NULL;
Animation*		g_Anim = NULL;
PhysicsManager*	g_physicsEngine = NULL;
int				g_activeAnimation = 0;
Morph*			g_Morph = NULL;
MultiMorph*		g_MultiMorph = NULL;
MorphMesh*		g_MorphSkeleton = NULL;
Face*			g_Face = NULL;
ComplexFace*	g_ComplexFace = NULL;
FaceModel*		g_FaceModel = NULL;
FaceModel*		g_FaceModelGenerate = NULL;
FaceController* g_FaceControllerGenerate = NULL;
D3DXVECTOR3		g_rayOrg;
D3DXVECTOR3		g_rayDir;	
FaceFacory*		g_pFaceFactory = NULL;
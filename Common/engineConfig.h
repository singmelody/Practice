#pragma once

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                      \
	{                                                  \
	HRESULT hr = x;                                \
	if(FAILED(hr))                                 \
		{                                              \
		DXTrace(__FILE__, __LINE__, hr, #x, TRUE); \
		}                                              \
	}
#endif

#else
#ifndef HR
#define HR(x) x;
#endif
#endif 

#define ILINE				__forceinline

#define DLL_EXPORT			__declspec(dllexport)
#define DLL_IMPORT			__declspec(dllimport)

#define MODULE_HANDLE       HMODULE

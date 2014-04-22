#include "DXUT.h"
#include "Utils.h"
#include <string>


wchar_t *GetWC(const char *c)
{
	const size_t cSize = strlen(c)+1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs (wc, c, cSize);

	return wc;
}

ID3DXEffect*    g_pEffect = NULL;       // D3DX effect interface
SkinnedMesh*	g_SkinnedMesh = NULL;
ID3DXLine*		g_Line = NULL;
Animation*		g_Anim = NULL;


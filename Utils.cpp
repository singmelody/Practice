#include "DXUT.h"
#include "Utils.h"
#include <string>
#include "Morph.h"

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

ID3DXEffect*    g_pEffect = NULL;       // D3DX effect interface
SkinnedMesh*	g_SkinnedMesh = NULL;
ID3DXLine*		g_Line = NULL;
Animation*		g_Anim = NULL;
PhysicsManager*	g_physicsEngine = NULL;
int				g_activeAnimation = 0;
Morph*			g_Morph = NULL;
MultiMorph*		g_MultiMorph = NULL;
MorphMesh*		g_MorphSkeleton = NULL;
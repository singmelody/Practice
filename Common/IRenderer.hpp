#pragma once

namespace Dream
{

class IRenderer
{
public:
	IRenderer(void){}
	virtual ~IRenderer(void){}

	virtual bool Init(const HWND mainHwnd) = 0;
	virtual void Destroy() = 0;

	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
};

   extern IRenderer*	gRenderer;
}

#define DEFINE_RENDER_INTERFACE  Dream::IRenderer*  Dream::gRenderer = NULL;

#define CREATE_RENDERER_DLL_FUNC   "CreateRenderer"
typedef void(*CreateRendererFunc)(Dream::IRenderer**);
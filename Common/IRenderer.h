#pragma once

namespace Dream
{

class IRenderer
{
public:
	IRenderer(void);
	~IRenderer(void);

	virtual void Init() = 0;
	virtual void Destroy() = 0;

	virtual void Update() = 0;
	virtual void Render() = 0;
};

}

#define DEFINE_RENDER_INTERFACE  Dream::IRenderer*  Dream::gRenderer = nullptr;

#define CREATE_RENDERER_DLL_FUNC   "Init"
typedef void(*Init)(Dream::IRenderer**);
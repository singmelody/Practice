#pragma once

#include <vector>
#include <algorithm>
#include "IRenderCommand.hpp"
#include <Windows.h>

namespace Dream
{
// declaration
class IEngine;
class IRenderDevice;
class IRenderCommand;
class IShader;

// helper
static bool compareRenderCommand(IRenderCommand* a, IRenderCommand* b)
{
	return a->GetGlobalOrder() < b->GetGlobalOrder();
}

class RenderQueue {

public:
	void push_back(IRenderCommand* command);
	size_t size() const;
	void sort()
	{
		// Don't sort _queue0, it already comes sorted
		std::sort(std::begin(m_queueNegZ), std::end(m_queueNegZ), compareRenderCommand);
		std::sort(std::begin(m_queuePosZ), std::end(m_queuePosZ), compareRenderCommand);
	}

	IRenderCommand* operator[](size_t index) const
	{
// 		if( index < m_queueNegZ.size() )
// 			return m_queueNegZ[index];
// 
// 		if( index < m_queue0.size() )
// 			return m_queue0[index];
// 
// 		index -= m_queue0.size();
// 
// 		if( index < )
	}

	void clear()
	{
		m_queueNegZ.clear();
		m_queue0.clear();
		m_queuePosZ.clear();
	}

protected:
	std::vector<IRenderCommand*> m_queueNegZ;
	std::vector<IRenderCommand*> m_queue0;
	std::vector<IRenderCommand*> m_queuePosZ;
};

class IRenderer
{
public:
	IRenderer() {}
	virtual ~IRenderer(){}

	virtual bool Init(const void* wnd, IEngine* engine) = 0;
	virtual void Destroy() = 0;

	virtual IRenderDevice* GetIRenderDevice() = 0;

	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual void SetCurShader(IShader* shader) = 0;

protected:
	std::vector<RenderQueue> m_renderGroups;
	std::vector<IRenderCommand*> m_batchedCommands;
};

   extern IRenderer*	gRenderer;
}

#define DEFINE_RENDER_INTERFACE  Dream::IRenderer*  Dream::gRenderer = NULL;

#define CREATE_RENDERER_DLL_FUNC   "CreateRenderer"
typedef void(*CreateRendererFunc)(Dream::IRenderer**);
#pragma once

#include <map>
#include <string>
#include "EngineConfig.h"

namespace Dream
{

	class IEngine;
	class IShader;

	class IShaderManager
	{
		typedef std::map<std::string, IShader*>	ShaderDic;
	public:
		
		IShaderManager() {}
		virtual ~IShaderManager(){
			for (ShaderDic::iterator itr = m_shaderDic.begin(); itr != m_shaderDic.end(); ++itr)
				SAFE_DELETE(itr->second);

			m_shaderDic.clear();
		}

		virtual bool Init(IEngine* ) = 0;
		virtual void Destroy() = 0;


		virtual IShader* GetShader(const char* name)
		{
			ShaderDic::iterator itr = m_shaderDic.find(name);
			if( itr != m_shaderDic.end() )
				return itr->second;
			
			IShader* tex = CreateShader(name);
			if(tex != NULL)
				m_shaderDic.insert(std::make_pair(name, tex));
		}

		virtual void UnloadShader(const char* name)
		{
			ShaderDic::iterator itr = m_shaderDic.find(name);
			if( itr != m_shaderDic.end() )
			{
				SAFE_DELETE(itr->second);
				m_shaderDic.erase(itr);
			}

		}

	protected:
		virtual IShader* CreateShader(const char* name) = 0;

		ShaderDic m_shaderDic;
	};

	extern IShaderManager*	gTextureManager;
}

#define DEFINE_TEXTUREMGR_INTERFACE  Dream::ITextureManager*  Dream::gTextureManager = NULL;

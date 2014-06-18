#pragma once
#include <map>
#include <string>
#include "EngineConfig.h"

namespace Dream
{
	typedef int TexID;
	const TexID invalidTexID = -1;

	class IEngine;
	class ITexture;

	class ITextureManager
	{
		typedef std::map<std::string,ITexture*>	TexDic;
	public:
		
		ITextureManager() {}
		virtual ~ITextureManager(){
			for (TexDic::iterator itr = m_texDic.begin(); itr != m_texDic.end(); ++itr)
				SAFE_DELETE(itr->second);

			m_texDic.clear();
		}

		virtual bool Init(IEngine* ) = 0;
		virtual void Destroy() = 0;


		virtual ITexture* GetTexture(const char* name)
		{
			TexDic::iterator itr = m_texDic.find(name);
			if( itr != m_texDic.end() )
				return itr->second;
			
			ITexture* tex = CreateTexture(name);
			if(tex != NULL)
				m_texDic.insert(std::make_pair(name, tex));
		}

		virtual void UnloadTex(const char* name)
		{
			TexDic::iterator itr = m_texDic.find(name);
			if( itr != m_texDic.end() )
			{
				SAFE_DELETE(itr->second);
				m_texDic.erase(itr);
			}

		}

	protected:
		virtual ITexture* CreateTexture(const char* name) = 0;

		TexDic m_texDic;
	};

	extern ITextureManager*	gTextureManager;
}

#define DEFINE_TEXTUREMGR_INTERFACE  Dream::ITextureManager*  Dream::gTextureManager = NULL;

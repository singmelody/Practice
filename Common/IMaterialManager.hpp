#pragma once

#include <map>
#include <string>
#include "EngineConfig.h"

namespace Dream
{

	class IEngine;
	class IMaterial;

	class IMaterialManager
	{
		typedef std::map<std::string, IMaterial*>	MaterialDic;
	public:
		
		IMaterialManager() {}
		virtual ~IMaterialManager(){
			for (MaterialDic::iterator itr = m_materialDic.begin(); itr != m_materialDic.end(); ++itr)
				SAFE_DELETE(itr->second);

			m_materialDic.clear();
		}

		virtual bool Init(IEngine* ) = 0;
		virtual void Destroy() = 0;


		virtual IMaterial* GetMaterial(const char* name)
		{
			MaterialDic::iterator itr = m_materialDic.find(name);
			if( itr != m_materialDic.end() )
				return itr->second;
			
			IMaterial* tex = CreateMaterial(name);
			if(tex != NULL)
				m_materialDic.insert(std::make_pair(name, tex));
		}

		virtual void UnloadMaterial(const char* name)
		{
			MaterialDic::iterator itr = m_materialDic.find(name);
			if( itr != m_materialDic.end() )
			{
				SAFE_DELETE(itr->second);
				m_materialDic.erase(itr);
			}

		}

		virtual IMaterial* CreateMaterial(const char* name) = 0;
	protected:

		MaterialDic m_materialDic;
	};

	extern IMaterialManager*	gMaterialManager;
}

#define DEFINE_MATERIALMGR_INTERFACE  Dream::IMaterialManager*  Dream::gMaterialManager = NULL;

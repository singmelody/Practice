#pragma once
#include <map>
#include <string>
namespace Dream
{
	typedef int TexID;
	const TexID invalidTexID = -1;

	class ITexture;

	class ITextureManager
	{
		typedef std::map<std::string,ITexture*>	TexDic;
	public:
		
		ITextureManager() {}
		virtual ~ITextureManager(){}

		virtual ITexture* GetTexture(const char* name)
		{
			TexDic::iterator itr = m_texDic.find(name);
			if( itr != m_texDic.end() )
				return itr->second;
			
			ITexture* tex = CreateTexture(name);
			if(tex != NULL)
				m_texDic.insert(std::make_pair(name, tex));
		}

		virtual ITexture* CreateTexture(const char* name) = 0;

	protected:
		TexDic m_texDic;
	};
}
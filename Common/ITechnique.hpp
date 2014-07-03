#pragma once
#include <vector>
namespace Dream{

	class IPass;

	class ITechnique
	{
	public:
		ITechnique(){}
		virtual ~ITechnique(){

			for (size_t i = 0; i < m_passes.size(); ++i)
			{
				if(m_passes[i] != NULL)
				{
					delete m_passes[i];
					m_passes[i] = NULL;
				}
			}

		}

		IPass* GetPass(const char * name, int i)
		{

			 return m_passes[i];
		}

		virtual void Render(const char* techName) = 0;


	protected:

		std::vector<IPass*> m_passes;
	};
}
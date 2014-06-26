#pragma once
#include <vector>
namespace Dream{

	class IPass;

	class ITechnique
	{
	public:
		ITechnique(){}
		virtual ~ITechnique(){

			for (int i = 0; i < m_passes.size(); ++i)
			{
				if(m_passes[i] != NULL)
				{
					delete m_passes[i];
					m_passes[i] = NULL;
				}
			}

		}
	protected:
		std::vector<IPass*> m_passes;
	};
}
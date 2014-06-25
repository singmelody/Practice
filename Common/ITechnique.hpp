#pragma once
#include <vector>
namespace Dream{

	class IPass;

	class ITechnique
	{
	public:
		ITechnique(){}
		virtual ~ITechnique(){}
	protected:
		std::vector<IPass*> m_passes;
	};
}
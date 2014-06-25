#pragma once
#include <vector>

namespace Dream
{
	class ITechnique;

	class IShader
	{
	public:


	protected:
		std::vector<ITechnique*> m_techniques;
	};
}
#pragma once
#include <vector>

namespace Dream
{
	class ITechnique;

	class IShader
	{
	public:
		enum PARAMTYPE
		{
			eFloat,
			eVector3,
			eVector4,
			eMatrix,
			eTexture
		};

		virtual void SetShaderParam(const char* varName, void* data, PARAMTYPE type) = 0;

	protected:
		std::vector<ITechnique*> m_techniques;
	};
}
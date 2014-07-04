#pragma once

namespace Dream
{
	class IShader;

	class IMaterial
	{
	public:
		IMaterial(){}
		virtual ~IMaterial(){}

	protected:
		IShader*	m_shader;
	};
}
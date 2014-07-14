#pragma once

namespace Dream
{
	class IShader;

	class IMaterial
	{
	public:
		IMaterial() : m_shader(0){}
		virtual ~IMaterial(){}

		virtual bool Load() = 0 ;

	protected:
		IShader*	m_shader;
	};
}
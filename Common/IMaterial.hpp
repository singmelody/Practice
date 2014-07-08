#pragma once

namespace Dream
{
	class IShader;

	class IMaterial
	{
	public:
		IMaterial(){}
		virtual ~IMaterial(){}

		virtual bool Load() = 0 ;

	protected:
		IShader*	m_shader;
	};
}
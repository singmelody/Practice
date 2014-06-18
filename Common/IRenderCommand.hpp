#pragma once

namespace Dream
{
	class IRenderCommand
	{
	public:
		enum Type
		{
			UNKNOWN_COMMAND,
			QUAD_COMMAND,
			CUSTOM_COMMAND,
			BATCH_COMMAND,
			GROUP_COMMAND,
			MESH_COMMAND,
		};

		virtual float GetGlobalOrder(){ return m_globalOrder; }

		virtual	Type getType() const { return m_type; }
	protected:
		IRenderCommand(): m_type(UNKNOWN_COMMAND), m_globalOrder(0) {}
		virtual ~IRenderCommand();

		float	m_globalOrder;
		Type	m_type;
	};
}
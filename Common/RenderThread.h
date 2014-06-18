#pragma once
#include <vector>
namespace Dream
{
	enum RenderCommand
	{
		eRC_Quad,
		eRC
	};

	class RenderThread
	{
	public:
		std::vector<byte> m_commands[];
	};
}
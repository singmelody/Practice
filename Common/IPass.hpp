#pragma once
#include <vector>
namespace Dream{

	class IPass
	{
	public:
		IPass(){}
		virtual ~IPass(){}

		virtual void BeginRender(void *handle, int i) = 0;
		virtual void EndRender(void *handle) = 0;

	protected:
	};
}
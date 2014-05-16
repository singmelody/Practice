#pragma once
class IRenderDevice
{
public:
	IRenderDevice(void);
	virtual ~IRenderDevice(void);

	virtual bool CreateDevice() = 0;

	virtual bool CheckCaps() = 0;
};


#pragma once
class IRenderDevice
{
public:
	IRenderDevice(void);
	virtual ~IRenderDevice(void);

	virtual bool CreateDevice() = 0;

	virtual bool CheckCaps() = 0;

	virtual bool CreateSwapChain() = 0;

	virtual UINT GetReference() = 0;
};


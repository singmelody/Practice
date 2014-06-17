// Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Demo.h"


int _tmain(int argc, _TCHAR* argv[])
{
	Dream::DemoApp app;
	if(app.Startup())
	{
		app.Run();
		app.Shutdown();
	}
	return 0;
}


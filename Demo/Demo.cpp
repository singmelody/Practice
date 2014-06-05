// Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <Windows.h>
#include <IEngine.h>

namespace Dream
{
	class DemoApp
	{
	public:
		DemoApp() : m_isRunning(false)
		{

		}

		bool	Startup()
		{
			if (!CreateWnd("Test", 800, 600))
				return false;

			const char* engineDll = "Engine.dll";
			m_engineDll = LoadLibrary(engineDll);
			if (!m_engineDll)
				return false;

			CreateEngineFunc func = (CreateEngineFunc)GetProcAddress( m_engineDll, CREATE_ENGINE_DLL_FUNC);
			if (!func)
				return false;
 
 			func(&gEngine);

// 			if (!gEngine)
// 				return false;

// 			EngineStartupInfo info;
// 			info.fullScreen = false;
// 			info.winHandle = _hwnd;
// 			info.winWidth = _width;
// 			info.winHeight = _height;
// 
// 			if (!gEngine->Startup(info))
// 				return false;
// 
// 			if (_currDemo)
// 				_currDemo->OnStartup();

			return true;
		}

		void Run()
		{
			while (m_isRunning)
			{
				while (!MessagePump())
				{
// 					if (_currDemo)
// 						_currDemo->OnUpdate(0.016f);

					gEngine->Update(0.016f);
				}
			}
		}

		void Shutdown()
		{

		}

	private:
		bool CreateWnd(const char* winName, int width, int height)
		{
			m_winName = winName;
			m_width = width;
			m_height = height;

			// Register the windows class
			WNDCLASS wndClass;
			wndClass.style = CS_DBLCLKS;
			wndClass.lpfnWndProc = WndProc;
			wndClass.cbClsExtra = 0;
			wndClass.cbWndExtra = 0;
			wndClass.hInstance = GetModuleHandle(NULL);
			wndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION);
			wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
			wndClass.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
			wndClass.lpszMenuName = NULL;
			wndClass.lpszClassName = m_winName.c_str();

			if (!RegisterClass(&wndClass))
				return false;

			// Create the render window
			m_hwnd = CreateWindow( m_winName.c_str(), m_winName.c_str(), WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, m_width, m_height, NULL,
				NULL, GetModuleHandle(NULL), NULL);

			if(!m_hwnd)
				return false;

			SetWindowLongPtr( m_hwnd, GWLP_USERDATA, (LONG)this);

			ShowWindow( m_hwnd, SW_SHOW);
			UpdateWindow(m_hwnd);

			m_isRunning = true;

			return true;
		}

		static  LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			static DemoApp*  demoApp = nullptr;

			if (!demoApp)
				demoApp = (DemoApp*)(::GetWindowLongPtrA(hwnd, GWLP_USERDATA));

			if (demoApp)
				demoApp->ProcessMsg(hwnd, message, wParam, lParam);

			return DefWindowProc(hwnd, message, wParam, lParam);
		}

		LRESULT CALLBACK ProcessMsg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch(message)
			{
			case WM_CLOSE:
				m_isRunning = false;
				PostQuitMessage(0);
				break;
			case WM_DESTROY:
				m_isRunning = false;
				PostQuitMessage(0);
				break;
			}

			return DefWindowProc(hwnd, message, wParam, lParam);
		}

		void DestroyWnd()
		{
			DestroyWindow(m_hwnd);
			::UnregisterClass( m_winName.c_str(), GetModuleHandle(NULL));
		}

		bool MessagePump()
		{
			MSG msg;
			msg.message = WM_NULL;
			bool hasMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0;
			if (hasMsg)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return hasMsg;
		}

		HMODULE m_engineDll;

		std::string m_winName;
		int m_width;
		int m_height;
		HWND m_hwnd;

		bool m_isRunning;
	};
}

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


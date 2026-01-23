#include "KWindow.h"

#include <windows.h>

namespace KE::SYSTEM
{
	KReturn KWindow::Create(WindowDesc& Desc)
	{
		//Creates the window
		CreateWin32Window(Desc);

	}

	KReturn KWindow::SetWindowTitle(const wchar_t title)
	{
		//Sets window title 
	}
	
	KReturn KWindow::CreateWin32Window(WindowDesc& Desc)
	{
		PtrLoader = GetModuleHandle(NULL);



		WNDCLASS wc = {};
		wc.hInstance = PtrLoader;
		wc.lpszClassName = reinterpret_cast<LPCWSTR>(Desc.ClassName);
		wc.lpfnWndProc = WindowProc;

		RegisterClass(&wc);

		WindowHandle = CreateWindowEx(0, Desc.ClassName, Desc.WindowTitle, 0, CW_USEDEFAULT
			, CW_USEDEFAULT, Desc.WindowWidth, Desc.WindowHeight, NULL, NULL, PtrLoader, NULL);

		if (WindowHandle = nullptr) return KReturn::K_WINDOW_CREATION_FAILED;

		ShowWindow(WindowHandle, 0);
		
		MessageDispatcher();
	}

//Static functions
	static void MessageDispatcher()
	{
		MSG msg = {};

		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	static LRESULT CALLBACK WindowProc(HWND Window, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_DESTROY:
			{
			PostQuitMessage(0);
			}
			break;
			case WM_CLOSE:
			{
			DestroyWindow(Window);
			}
			break;
			case WM_SIZE:
			{
			RECT ClientRect;
			GetClientRect(Window, &ClientRect);
			int width = ClientRect.right - ClientRect.left;
			int height = ClientRect.bottom - ClientRect.top;
			}
			break;
			case WM_PAINT:
			{

				PAINTSTRUCT Paint;
				HDC DeviceContext = BeginPaint(Window, &Paint);
				RECT ClientRECT;
				GetClientRect(Window, &ClientRECT);
				//EndPaint(hwnd, &Paint);
			}
			break;
		}

		return DefWindowProc(Window, message, wParam, lParam);
	}
}
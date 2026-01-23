#include "KWindow.h"

#include <windows.h>

namespace KE::SYSTEM
{
	void KWindow::Create()
	{
		//Creates the window
		CreateWin32Window(_WindowDesc);
	}

	KReturn KWindow::SetWindowDesc(const WCHAR* WindowClassName, const WCHAR* WindowTitle, 
		const int WindowWidth, const int WindowHeight)
	{
		KE::SYSTEM::WindowDesc desc = {};

		 desc.ClassName = WindowClassName;
		desc.WindowTitle = WindowTitle;
		desc.WindowWidth = WindowWidth;
		desc.WindowHeight = WindowHeight;
		
		_WindowDesc = desc;

		return KReturn::K_WINDOW_CREATION_SUCCESS;
	}
	
	void KWindow::CreateWin32Window(WindowDesc& Desc)
	{
		PtrLoader = GetModuleHandle(NULL);
		WNDCLASS wc = {};
		wc.hInstance = PtrLoader;
		wc.lpszClassName = reinterpret_cast<LPCWSTR>(Desc.ClassName);
		wc.lpfnWndProc = ::WindowProc;

		RegisterClass(&wc);

		WindowHandle = CreateWindowEx(0, Desc.ClassName, Desc.WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT
			, CW_USEDEFAULT, Desc.WindowWidth, Desc.WindowHeight, NULL, NULL, PtrLoader, NULL);

		if (WindowHandle == nullptr) return;

		ShowWindow(WindowHandle, SW_SHOW);
	}

	KReturn KWindow::MessageDispatcher()
	{
		MSG msg = {};

		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return KReturn::K_PROGRAM_CLOSED;
	}


}


//Static Functions
LRESULT CALLBACK WindowProc(HWND Window, UINT message, WPARAM wParam, LPARAM lParam)
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
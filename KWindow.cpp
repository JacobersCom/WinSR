#include "KWindow.h"

#include <windows.h>

namespace KE::SYSTEM
{
	KReturn KWindow::Create()
	{
		//Creates the window
		CreateWin32Window();

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
		wc.lpfnWndProc = 
	}
	
	LRESULT KWindow::WindowProc(HWND Window, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}break;
		case WM_CLOSE:
		{
			DestroyWindow(Window);
		}break;
		case WM_SIZE:
		{
			RECT ClientRect;
			GetClientRect(Window, &ClientRect);
			int width = ClientRect.right - ClientRect.left;
			int height = ClientRect.bottom - ClientRect.top;
			break;
		}
		case WM_PAINT:
		{

			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			RECT ClientRECT;
			GetClientRect(Window, &ClientRECT);
			//EndPaint(hwnd, &Paint);
			break;
		}
		}

		return DefWindowProc(Window, message, wParam, lParam);
	}
}
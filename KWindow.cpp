#include "KWindow.h"

#include <windows.h>

namespace KE::SYSTEM
{
	void KWindow::Create()
	{
		//Creates the window
		CreateWin32Window(_WindowDesc);
	}

	void KWindow::SetWindowDesc(const WCHAR* WindowClassName, const WCHAR* WindowTitle, 
		const int WindowWidth, const int WindowHeight)
	{
		KE::SYSTEM::WindowDesc desc = {};

		 desc.ClassName = WindowClassName;
		desc.WindowTitle = WindowTitle;
		desc.WindowWidth = WindowWidth;
		desc.WindowHeight = WindowHeight;
		
		_WindowDesc = desc;

	
	}

	void KWindow::GetFrameBufferSize(HWND WindowHandle, int& Width, int& Height)
	{
		RECT ClientRect;
		GetClientRect(WindowHandle, &ClientRect);

		//Top left screen coord
		POINT p0{ ClientRect.left, ClientRect.top };
		//Bottom right screen coord
		POINT p1{ ClientRect.right, ClientRect.bottom };

		using WinFp = BOOL(WINAPI*)(HWND, LPPOINT);

		HMODULE User32 = GetModuleHandleW(L"User32.dll");

		//Returns address of function else returns nullptr. 
		auto logicalToPhysicalPM = (WinFp)GetProcAddress(User32, "LogicalToPhysicalPointForPerMonitorDPI");

		if (logicalToPhysicalPM)
		{
			logicalToPhysicalPM(WindowHandle, &p0);
			logicalToPhysicalPM(WindowHandle, &p1);

		}

		Width = (int)(p1.x - p0.x);
		Height = (int)(p1.y - p0.y);
	}
	
	KReturn KWindow::CreateWin32Window(WindowDesc& Desc)
	{
		PtrLoader = GetModuleHandle(NULL);
		WNDCLASS wc = {};
		wc.hInstance = PtrLoader;
		wc.lpszClassName = reinterpret_cast<LPCWSTR>(Desc.ClassName);
		wc.lpfnWndProc = ::WindowProc;

		RegisterClass(&wc);

		WindowHandle = CreateWindowEx(0, Desc.ClassName, Desc.WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT
			, CW_USEDEFAULT, Desc.WindowWidth, Desc.WindowHeight, NULL, NULL, PtrLoader, NULL);

		if (WindowHandle == nullptr) return KReturn::K_WINDOW_CREATION_FAILED;

		ShowWindow(WindowHandle, SW_SHOW);

		return KReturn::K_WINDOW_CREATION_SUCCESS;
	}

	KReturn KWindow::EventDispatcher()
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
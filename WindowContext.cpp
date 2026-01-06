#include "WindowContext.hpp"

static LRESULT WindowProc(HWND Window, UINT message, WPARAM wParam, LPARAM lParam)
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
		Win32ResizeDIBSection(width, height);
		break;
	}
	case WM_PAINT:
	{

		PAINTSTRUCT Paint;
		HDC DeviceContext = BeginPaint(Window, &Paint);
		RECT ClientRECT;
		GetClientRect(Window, &ClientRECT);
		Win32UpdateWindow(DeviceContext, &ClientRECT);
		//EndPaint(hwnd, &Paint);
		break;
	}
	}

	return DefWindowProc(Window, message, wParam, lParam);
}

bool WindowContext::RegisterWindow(HINSTANCE window, int show, const WCHAR* className, 
	const WCHAR* windowName, int width, int height)
{
	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = window;
	wc.lpszClassName = className;

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0, //Optional window style
		className, //window class name
		windowName, //Window title
		WS_OVERLAPPEDWINDOW, // Window style
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, //Size and position
		NULL, //Parent window
		NULL, //Meun
		window, //Instance handler
		NULL //Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, show);

	std::cout << "Window created: " << hwnd << "\n";

    return false;
}


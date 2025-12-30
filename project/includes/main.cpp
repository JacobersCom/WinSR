#include <Windows.h>
#include <sal.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance, 
	_In_ HINSTANCE prevInstance, 
	_In_ LPSTR, 
	_In_ int show)
{
	const wchar_t className[] = L"Sample Window Class";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0, //Optional window style
		className, //window class name
		L"Raster Surface", //Window title
		WS_OVERLAPPEDWINDOW, // Window style
		CW_USEDEFAULT, CW_USEDEFAULT, 700, 700, //Size and position
		NULL, //Parent window
		NULL, //Meun
		hInstance, //Instance handler
		NULL //Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, show);

	MSG msg = {};

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg); //Always called before dispatch
		DispatchMessage(&msg); //Alows the system to ignore or reply to the message
	}
	return 0;
}

LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_QUIT :
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}



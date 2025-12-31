#include <Windows.h>


#define global_var static
#define local_persist static
#define internal static

global_var bool running;
global_var BITMAPINFO BitMapInfo;
global_var void* BitMapMemory;
global_var HBITMAP BitMapHandle;
global_var HDC BitMapDeviceContext;

struct DibSectionResutls
{
	HBITMAP handle;
	void* Memory;
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

internal //Local to this file only
void Win32ResizeDIBSection(int width, int height);
internal
void Win32UpdateWindow(HDC DeviceContext, int x, int y, int width, int height);

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
		case WM_DESTROY :
		{
			PostQuitMessage(0);
		}break;
		case WM_CLOSE :
		{
			DestroyWindow(hwnd);
		}break;
		case WM_SIZE :
		{
			RECT ClientRect;
			GetClientRect(hwnd, &ClientRect);
			int width = ClientRect.right - ClientRect.left;
			int height = ClientRect.bottom - ClientRect.top;
			Win32ResizeDIBSection(width, height);
			break;
		}
		case WM_PAINT :
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(hwnd, &Paint);
			int x = Paint.rcPaint.left;
			int y = Paint.rcPaint.top;
			int width = Paint.rcPaint.right - Paint.rcPaint.left;
			int height = Paint.rcPaint.bottom - Paint.rcPaint.top;
			local_persist DWORD Operation = WHITENESS;
			PatBlt(DeviceContext, x, y, width, height, Operation);
			Win32UpdateWindow(DeviceContext, x, y, width, height);
			EndPaint(hwnd, &Paint);
			break;
		}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}


void Win32ResizeDIBSection(int width, int height)
{
	if (BitMapHandle)
	{
		DeleteObject(BitMapHandle);
	}

	if (!BitMapDeviceContext)
	{
		BitMapDeviceContext = CreateCompatibleDC(0);
	}

	BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);
	BitMapInfo.bmiHeader.biWidth = width;
	BitMapInfo.bmiHeader.biHeight = height;
	BitMapInfo.bmiHeader.biPlanes = 1;
	BitMapInfo.bmiHeader.biBitCount = 32; //bit count prepixel
	BitMapInfo.bmiHeader.biCompression = BI_RGB;

	BitMapHandle = CreateDIBitmap(BitMapDeviceContext, &BitMapInfo.bmiHeader, DIB_RGB_COLORS, &BitMapMemory, &BitMapInfo, 0);

	
}

void Win32UpdateWindow(HDC DeviceContext, int x, int y, int width, int height)
{
	//Rect to rect image copy, if the des is bigger the image is increased
	StretchDIBits(
		DeviceContext,
		x, y, width, height,
		x, y, width, height,
		BitMapMemory, 
		&BitMapInfo,
		DIB_RGB_COLORS, SRCCOPY
	);
}


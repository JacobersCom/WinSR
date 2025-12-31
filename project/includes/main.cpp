#include <Windows.h>
#include <stdio.h >


#define global_var static
#define local_persist static
#define internal static

global_var bool running;
global_var BITMAPINFO BitMapInfo;
//Using a void* to repersent new allocated memory
global_var void* BitMapMemory;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

internal //Local to this file only
void Win32ResizeDIBSection(int width, int height);
internal //Local to this file only
void Win32UpdateWindow(HDC DeviceContext, int x, int y, int width, int height);
internal
void ErrorExit();


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
	if (BitMapMemory)
	{
		VirtualFree(BitMapMemory, 0, MEM_RELEASE);
	}

	//Amount of bytes prepixel
	int BytesPrePixel = 4;
	//total pixels of rect 
	int BitMapMemorySize = BytesPrePixel * (width * height);
	//Using MEM_COMMIT and MEM_RESERVE to commit the page to memory for later reading/writing
	//which is what page_readwrite is for
	BitMapMemory = VirtualAlloc(0, BitMapMemorySize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	
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



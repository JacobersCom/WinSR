#include <Windows.h>
#include <stdio.h >
#include <iostream>

#include "Renderer.hpp"


#define wW 1024
#define wH 700

#define global_var static
#define local_persist static
#define internal static

//Remove all global vars
global_var bool running;
global_var BITMAPINFO BitMapInfo;
//Using a void* to repersent new allocated memory
global_var void* BitMapMemory;
global_var int BytePrePixel;
global_var int BitMapWidth;
global_var int BitMapHeight;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

internal //Local to this file only
void Win32ResizeDIBSection(int width, int height);
internal //Local to this file only
void Win32UpdateWindow(HDC DeviceContext, RECT* ClientRECT);
internal
void Renderfun(int x, int y);

#if _DEBUG
internal
void CreateConsoleWindow()
{
	if (!AttachConsole(ATTACH_PARENT_PROCESS))
	{
		//Creates a console window
		AllocConsole();
	}

	FILE* stream;

	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONOUT$", "w", stderr);
	freopen_s(&stream, "CONIN$", "r", stdin);

	std::ios::sync_with_stdio(true);

	std::cout << "Debug Window ready.\n";
};
#endif

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
		CW_USEDEFAULT, CW_USEDEFAULT, wW, wH, //Size and position
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

#if _DEBUG
CreateConsoleWindow();
#endif // DEBUG


	std::cout << "Window created: " << hwnd << "\n";

	MSG msg = {};

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg); //Always called before dispatch
		DispatchMessage(&msg); //Alows the system to ignore or reply to the message
	}

	return 0;
}

LRESULT WindowProc(HWND Window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY :
		{
			PostQuitMessage(0);
		}break;
		case WM_CLOSE :
		{
			DestroyWindow(Window);
		}break;
		case WM_SIZE :
		{
			RECT ClientRect;
			GetClientRect(Window, &ClientRect);
			int width = ClientRect.right - ClientRect.left;
			int height = ClientRect.bottom - ClientRect.top;
			Win32ResizeDIBSection(width, height);
			break;
		}
		case WM_PAINT :
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


void Win32ResizeDIBSection(int width, int height)
{	
	if (BitMapMemory)
	{
		VirtualFree(BitMapMemory, 0, MEM_RELEASE);
	}

	BitMapWidth = width;
	BitMapHeight = height;

	BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);
	BitMapInfo.bmiHeader.biWidth = BitMapWidth;
	BitMapInfo.bmiHeader.biHeight = BitMapHeight;
	BitMapInfo.bmiHeader.biPlanes = 1;
	BitMapInfo.bmiHeader.biBitCount = 32; //bit count prepixel
	BitMapInfo.bmiHeader.biCompression = BI_RGB;

	//Amount of bytes prepixel
	BytePrePixel = 4;
	//total pixels of rect 
	unsigned int BitMapMemorySize = BytePrePixel * (BitMapWidth * BitMapHeight);
	//Using MEM_COMMIT and MEM_RESERVE to commit the page to memory for later reading/writing
	//which is what page_readwrite is for
	BitMapMemory = VirtualAlloc(0, BitMapMemorySize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	
	ParamertieLine(0, 0, width / 2, height / 2, 0x0000ff, 
		BitMapMemory, BytePrePixel, BitMapWidth);
}

void Win32UpdateWindow(HDC DeviceContext, RECT* ClientRECT)
{
	//Rect to rect image copy, if the des is bigger the image is increased
	int WindowWidth = ClientRECT->right - ClientRECT->left;
	int WindowHeight = ClientRECT->bottom - ClientRECT->top;

	StretchDIBits(
		DeviceContext,
		0, 0, WindowWidth, WindowHeight,
		0, 0, BitMapWidth, BitMapHeight,
		BitMapMemory, 
		&BitMapInfo,
		DIB_RGB_COLORS, SRCCOPY
	);
}

void Renderfun(int _x, int _y)
{
	uint32_t pitch = BitMapWidth * BytePrePixel;
	uint8_t* Row = (uint8_t*)BitMapMemory;


	for (int y = 0; y < BitMapHeight; ++y)
	{
		//Point Pixel at current row
		uint8_t* Pixel = (uint8_t*)Row;

		for (int x = 0; x < BitMapWidth; ++x)
		{
			//Move to next pixel

			//Blue byte 0 
			*Pixel = (uint8_t)(x + _x);
			//Move one byte
			Pixel++;

			//Green byte 1
			*Pixel = (uint8_t)(x+_x);
			++Pixel;

			//Red Byte 2
			*Pixel = (uint8_t)(x+_x);
			++Pixel;

			//Alpha Byte 3
			*Pixel = 0;
			++Pixel;
		}
		//Going to the next "Row"
		Row += pitch;
	}
}



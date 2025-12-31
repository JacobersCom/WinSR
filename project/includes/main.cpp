#include <Windows.h>
#include <stdio.h >
#include <stdint.h>


#define global_var static
#define local_persist static
#define internal static

//Remove all global vars
global_var bool running;
global_var BITMAPINFO BitMapInfo;
//Using a void* to repersent new allocated memory
global_var void* BitMapMemory;
global_var int BitMapWidth;
global_var int BitMapHeight;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

internal //Local to this file only
void Win32ResizeDIBSection(int width, int height);
internal //Local to this file only
void Win32UpdateWindow(HDC DeviceContext, RECT* ClientRECT);
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
			RECT ClientRECT;
			GetClientRect(hwnd, &ClientRECT);
			Win32UpdateWindow(DeviceContext, &ClientRECT);
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

	BitMapWidth = width;
	BitMapHeight = height;

	BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);
	BitMapInfo.bmiHeader.biWidth = BitMapWidth;
	BitMapInfo.bmiHeader.biHeight = -BitMapHeight;
	BitMapInfo.bmiHeader.biPlanes = 1;
	BitMapInfo.bmiHeader.biBitCount = 32; //bit count prepixel
	BitMapInfo.bmiHeader.biCompression = BI_RGB;

	//Amount of bytes prepixel
	int BytesPrePixel = 4;
	//total pixels of rect 
	unsigned int BitMapMemorySize = BytesPrePixel * (BitMapWidth * BitMapHeight);
	//Using MEM_COMMIT and MEM_RESERVE to commit the page to memory for later reading/writing
	//which is what page_readwrite is for
	BitMapMemory = VirtualAlloc(0, BitMapMemorySize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	
	uint32_t pitch = width * BytesPrePixel;
	uint8_t* Row = (uint8_t*) BitMapMemory;


	for (int y = 0; y < BitMapHeight; ++y)
	{
		//Point Pixel at current row
		uint8_t* Pixel = (uint8_t*)Row;

		for (int x = 0; x < BitMapWidth; ++x)
		{
			//Move to next pixel

			//Red byte 0 
			*Pixel = 0xFF;
			//Move one byte
			++Pixel;

			//Blue byte 1
			*Pixel = 0x00;
			++Pixel;

			//Green Byte 2
			*Pixel = 0x00;
			++Pixel;

			//Alpha Byte 3
			*Pixel = 0x00;
			++Pixel;
		}
		//Going to the next "Row"
		Row += pitch;
	}
	
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



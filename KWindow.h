#pragma once

#include <Windows.h>
#include "KReturn.h"

//Namespace holds data for making the window
namespace KE::SYSTEM
{

	struct WindowDesc
	{
		const WCHAR* ClassName;
		const WCHAR* WindowTitle;
		int WindowWidth;
		int WindowHeight;

	};
	
	//Class holds functions and private memebers for class
	class KWindow
	{
	public:

		void Create();
		void SetWindowDesc(const WCHAR* WindowClassName, const WCHAR* WindowTitle,
			const int WindowWidth, const int WindowHeight);
		KReturn EventDispatcher();

		const HWND GetWindowHandle() { return WindowHandle; }
		const HINSTANCE GetWindowInstance() { return PtrLoader; }

	private:

		KReturn CreateWin32Window(WindowDesc& desc);

	private:

		//ptr to where the DLL is held
		const HINSTANCE PtrLoader;

		//Handle to the window
		const HWND WindowHandle;

		WindowDesc _WindowDesc;
	};

};

LRESULT CALLBACK WindowProc(HWND Window, UINT message, WPARAM wParam, LPARAM lParam);
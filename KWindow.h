#include "KReturn.h"

//Namespace holds data for making the window
namespace KE::SYSTEM
{

	struct WindowDesc
	{
		const wchar_t ClassName;
		const int WindowWidth;
		const int WindowHeight;

	};
	


	//Class holds functions and private memebers for class
	class KWindow
	{
	public:

		KReturn Create();
		KReturn SetWindowTitle(const wchar_t Title);

	private:

		KReturn CreateWin32Window(WindowDesc& desc);

		static LRESULT WindowProc(HWND Window, UINT message, WPARAM wParam, LPARAM lParam);

	private:

		//ptr to where the DLL is held
		HINSTANCE PtrLoader = nullptr;

		//Handle to the window
		HWND WindowHandle = nullptr;
	};

};
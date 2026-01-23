#include "KReturn.h"

//Namespace holds data for making the window
namespace KE::SYSTEM
{

	struct WindowDesc
	{
		const WCHAR* ClassName = L"KOS Window";
		const WCHAR* WindowTitle = L"KOS Engine";
		const int WindowWidth = 500;
		const int WindowHeight = 500;

	};
	


	//Class holds functions and private memebers for class
	class KWindow
	{
	public:

		KReturn Create(WindowDesc& Desc);
		KReturn SetWindowTitle(const wchar_t Title);

	private:

		KReturn CreateWin32Window(WindowDesc& desc);

	private:

		//ptr to where the DLL is held
		HINSTANCE PtrLoader = nullptr;

		//Handle to the window
		HWND WindowHandle = nullptr;
	};

};
#include "KWindow.h"

#include <windows.h>

namespace KE::SYSTEM::KWindow
{
	KReturn KWindow::Create()
	{
		//Creates the window

		HINSTANCE WindowInstance = GetModuleHandle(NULL);

		STARTUPINFO StartUpInfo;
		GetStartupInfo(&StartUpInfo);
	}

	KReturn KWindow::SetWindowTitle(const wchar_t title)
	{
		//Sets window title 
	}
}
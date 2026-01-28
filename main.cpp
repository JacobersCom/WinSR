#include "KWindow.h"

int main()
{
	KE::SYSTEM::KWindow _win;

	_win.SetWindowDesc(L"KOS", L"KOS Engine", 500, 500);
	_win.Create();
	_win.EventDispatcher();
}
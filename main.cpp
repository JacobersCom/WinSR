
#include "KWindow.h"
#include "KRender.h"


int main()
{
	KE::SYSTEM::KWindow _win;
	KE::SYSTEM::KRender _render;

	_win.SetWindowDesc(L"KOS", L"KOS Engine", 500, 500);

	_render.run();

	KE::VULKAN::CreateWin32Surface(_win.GetWindowHandle(), _win.GetWindowInstance(), _render.GetVkInstance());

	_win.Create();
	_win.EventDispatcher();
}
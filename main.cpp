
#include "KWindow.h"
#include "KRender.h"


int main()
{
	KE::SYSTEM::KWindow _win;
	KE::RENDERER::KRender _render;

	_win.SetWindowDesc(L"KOS", L"KOS Engine", 500, 500);

	_render.run();

	_win.Create();
	_win.EventDispatcher();
}
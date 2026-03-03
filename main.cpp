
#include "KWindow.h"
#include "KRender.h"
#include "KVector3.h"
#include "KMat4.h"

#include <iostream>

int main()
{
	KE::SYSTEM::KWindow _win;
	KE::RENDERER::KRender _render(_win);
	KE::KOMATH::Mat4x4 mat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	std::cout << mat;

	_win.SetWindowDesc(L"KOS", L"KOS Engine", 500, 500);
	_win.Create();
	_render.run();
	_win.EventDispatcher();

	
}
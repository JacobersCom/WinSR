
#include "KWindow.h"
#include "KRender.h"
#include "KVector3.h"

#include <iostream>

int main()
{
	KE::SYSTEM::KWindow _win;
	KE::RENDERER::KRender _render(_win);
	KE::KOMATH::Vec3D _vec3(1.0f, 1.0f, 1.0f);
	std::cout << _vec3;
	_vec3 += 1.0;
	std::cout << _vec3;

	_win.SetWindowDesc(L"KOS", L"KOS Engine", 500, 500);
	_win.Create();
	_render.run();
	_win.EventDispatcher();

	
}
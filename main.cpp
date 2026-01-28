
#include "KWindow.h"
#include "KRender.h"


int main()
{
	KE::SYSTEM::KWindow _win;
	KE::SYSTEM::KRender _render;

	_win.SetWindowDesc(L"KOS", L"KOS Engine", 500, 500);

	//Vulkan surface creation
	VkWin32SurfaceCreateInfoKHR _WinSurfaceInfo{};

	_WinSurfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	_WinSurfaceInfo.hwnd = _win.GetWindowHandle();
	_WinSurfaceInfo.hinstance = _win.GetWindowInstance();

	VkSurfaceKHR surface = nullptr;

	vkCreateWin32SurfaceKHR(_render.GetVkInstance(), &_WinSurfaceInfo, nullptr, &surface);


	_render.run();
	_win.Create();
	_win.EventDispatcher();
}
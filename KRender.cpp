#include "KRender.h"


namespace KE::SYSTEM
{
	void KRender::run()
	{
		InitVulkan();
		UpdateLoop();
		CleanUp();
	}

	KReturn KRender::InitVulkan() 
	{
		KE::VULKAN::CreateVkInstance(_VkInstance);
		KE::VULKAN::CreateWin32Surface(_win.GetWindowHandle(), _win.GetWindowInstance(), _VkInstance);
		KE::VULKAN::FindQueueFamilies(_VkPhyscialDevice);
		return KE::KReturn::K_SUCCESS;
	}

	void KRender::UpdateLoop()
	{

	}

	void KRender::CleanUp()
	{

	}

}
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
		KE::VULKAN::SetUpDebuggerMessage();
		KE::VULKAN::CreateWin32Surface(_win.GetWindowHandle(), _win.GetWindowInstance(), _VkInstance, _VkSurface);
		KE::VULKAN::PickPhyicalDevice(_VkPhyscialDevice, _VkInstance);
		KE::VULKAN::FindQueueFamilies(_VkPhyscialDevice);
		KE::VULKAN::CreateLogicalDevice(_VkPhyscialDevice, _VkDevice, _VkQueue);
		return KE::KReturn::K_SUCCESS;
	}

	void KRender::UpdateLoop()
	{

	}

	void KRender::CleanUp()
	{

		vkDestroySurfaceKHR(_VkInstance, _VkSurface, nullptr);
		vkDestroyDevice(_VkDevice, nullptr);
	}

}
#include "KRender.h"


namespace KE::RENDERER
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
		KE::VULKAN::CreateWin32Surface(_win.GetWindowHandle(), _win.GetWindowInstance(), _VkInstance, _VkSurface);
		KE::VULKAN::PickPhyicalDevice(_VkPhyscialDevice, _VkInstance);
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

	KReturn KRender::CreateVkInstance(VkInstance& _VkInstance)
	{
		//Applcation information
		VkApplicationInfo AppInfo{};

		AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		AppInfo.pNext = VK_NULL_HANDLE;
		AppInfo.pApplicationName = "KOS Engine";
		AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		AppInfo.pEngineName = "KOS";
		AppInfo.apiVersion = VK_API_VERSION_1_4;

		VkInstanceCreateInfo InstanceInfo{};

		auto extentions = GetRequiredInstanceExtentions();

		InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		InstanceInfo.pApplicationInfo = &AppInfo;
		InstanceInfo.pNext = VK_NULL_HANDLE;
		InstanceInfo.enabledExtensionCount = static_cast<uint32_t>(extentions.size());
		InstanceInfo.ppEnabledExtensionNames = extentions.data();

		if (enableValidationLayers && !CheckValidationLayerSupport())
		{
			throw std::runtime_error("Validation layers requested, but not available");
		}
		else
		{
			InstanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			InstanceInfo.ppEnabledLayerNames = validationLayers.data();
		}


		vkCreateInstance(&InstanceInfo, nullptr, &_VkInstance);

		return KE::KReturn::K_SUCCESS;
	}

	KE::KReturn KRender::CreateWin32Surface(HWND _windowHandle, HINSTANCE _windowInstance, VkInstance _VkInstance, VkSurfaceKHR& _VkSurfaceKHR)
	{
		VkWin32SurfaceCreateInfoKHR _WinSurfaceInfo{};

		_WinSurfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		_WinSurfaceInfo.hwnd = _windowHandle;
		_WinSurfaceInfo.hinstance = _windowInstance;


		VkResult result = vkCreateWin32SurfaceKHR(_VkInstance, &_WinSurfaceInfo, nullptr, &_VkSurface);

		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Win32 Surface creation failed");
		}

		return KE::KReturn::K_SUCCESS;
	}

}
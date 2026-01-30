#include "vkHelper.h"

KE::KReturn CreateVkInstance(VkInstance& _VkInstance)
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

	auto extentions = GetRequiredExtentions();

	InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceInfo.pApplicationInfo = &AppInfo;
	InstanceInfo.pNext = VK_NULL_HANDLE;
	InstanceInfo.enabledExtensionCount = extentions.size();
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

KE::KReturn CreateWin32Surface(HWND _windowHandle, HINSTANCE _windowInstance, VkInstance _VkInstance)
{
	VkWin32SurfaceCreateInfoKHR _WinSurfaceInfo{};

	_WinSurfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	_WinSurfaceInfo.hwnd = _windowHandle;
	_WinSurfaceInfo.hinstance = _windowInstance;

	VkSurfaceKHR surface;

	VkResult result = vkCreateWin32SurfaceKHR(_VkInstance, &_WinSurfaceInfo, nullptr, &surface);
	if (result != VK_SUCCESS)
	{
		std::cerr << "Failed to create WindowSurface";
	}

	return KE::KReturn::K_SUCCESS;
}

bool CheckValidationLayerSupport()
{
	uint32_t layerCount;

	
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);

	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName))
			{
				layerFound = true;
				break;
			}
		}
		if (!layerFound)
		{
			return false;
		}
	}
	return true;
}

std::vector<const char*> GetRequiredExtentions()
{
	std::vector<const char*> extentions;

	if (enableValidationLayers)
	{
		extentions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	extentions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	extentions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);


	return extentions;
}
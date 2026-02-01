#include "vkHelper.h"

namespace KE::VULKAN
{
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
			throw std::runtime_error("Win32 Surface creation failed");
		}

		return KE::KReturn::K_SUCCESS;
	}

	KE::KReturn PickPhyicalDevice(VkPhysicalDevice& _VkPhysicalDevice, VkInstance _VkInstance)
	{
		//Get device count
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(_VkInstance, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			throw std::runtime_error("No device with vulkan support found!");
		}

		//Get device information
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(_VkInstance, &deviceCount, devices.data());

		//Find a suitable device with vulkan support
		for (const auto device : devices)
		{
			if (IsDeviceSuitable(device))
			{
				_VkPhysicalDevice = device;
				break;
			}
		}

		if (_VkPhysicalDevice == VK_NULL_HANDLE)
		{
			throw std::runtime_error("Failed to find Suitable GPU");
		}

		return KE::KReturn::K_SUCCESS;
	}

	//Logic to find graphics queue family
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _VkPhysicalDevice)
	{
		QueueFamilyIndices indices;

		//Get the properties count
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_VkPhysicalDevice, &queueFamilyCount, nullptr);

		//Get the properties data
		std::vector<VkQueueFamilyProperties> queueFamilys(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_VkPhysicalDevice, &queueFamilyCount, queueFamilys.data());

		//Find the graphics bit queue family
		for (const auto& queueFamily : queueFamilys)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.GraphicsFamily = indices.QueueCount;
			}

			if (indices.isComplete()) break;
			indices.QueueCount++;
		}

		return indices;
	}

	QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice _VkPhysicalDevice)
	{
		QueueFamilyIndices indices;
		return indices;
	}

	KE::KReturn CreateLogicalDevice(VkPhysicalDevice _VkPhysicalDevice)
	{
		float QueuePriority = 1.0f;
		QueueFamilyIndices indices = FindQueueFamilies(_VkPhysicalDevice);

		VkDeviceQueueCreateInfo DeviceQueueInfo{};
		DeviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		DeviceQueueInfo.pNext = VK_NULL_HANDLE;
		DeviceQueueInfo.queueCount = indices.QueueCount;
		DeviceQueueInfo.queueFamilyIndex = indices.GraphicsFamily.has_value();
		DeviceQueueInfo.pQueuePriorities = &QueuePriority; 

		VkPhysicalDeviceFeatures DeviceFeaturesInfo{};

		VkDeviceCreateInfo DeviceInfo{};
		DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		DeviceInfo.queueCreateInfoCount = 1;
		DeviceInfo.pQueueCreateInfos = &DeviceQueueInfo;
		DeviceInfo.pEnabledFeatures = &DeviceFeaturesInfo;

		return KE::KReturn();
	}

}

static bool CheckValidationLayerSupport()
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

static std::vector<const char*> GetRequiredExtentions()
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

//If the device has the properties and feature needed
static bool IsDeviceSuitable(VkPhysicalDevice _VkPhysicalDevice)
{
	VkPhysicalDeviceProperties _VkDeviceProperties;
	VkPhysicalDeviceFeatures _VkDeviceFeatures;

	vkGetPhysicalDeviceProperties(_VkPhysicalDevice, &_VkDeviceProperties);
	vkGetPhysicalDeviceFeatures(_VkPhysicalDevice, &_VkDeviceFeatures);

	return _VkDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		_VkDeviceFeatures.geometryShader;
}

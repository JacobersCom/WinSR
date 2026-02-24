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
		KE::RENDERER::KRender::CreateVkInstance(_VkInstance);
		KE::RENDERER::KRender::CreateWin32Surface(_win, _VkInstance, _VkSurface);
		KE::RENDERER::KRender::PickPhysicalDevice(_VkPhyscialDevice, _VkInstance);
		KE::RENDERER::KRender::CreateLogicalDevice(_VkPhyscialDevice, _VkDevice);
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

		if (enableValidationLayers)
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

	KE::KReturn KRender::CreateWin32Surface(KE::SYSTEM::KWindow& _win, VkInstance _VkInstance, VkSurfaceKHR& _VkSurfaceKHR)
	{

		VkWin32SurfaceCreateInfoKHR _WinSurfaceInfo{};

		_WinSurfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		_WinSurfaceInfo.hwnd = _win.GetWindowHandle();
		_WinSurfaceInfo.hinstance = _win.GetWindowInstance();


		VkResult result = vkCreateWin32SurfaceKHR(_VkInstance, &_WinSurfaceInfo, nullptr, &_VkSurface);

		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Win32 Surface creation failed");
		}

		return KE::KReturn::K_SUCCESS;
	}

	KE::KReturn KRender::PickPhysicalDevice(VkPhysicalDevice& _VkPhysicalDevice, VkInstance _VkInstance)
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

	KE::RENDERER::QueueFamilyIndices KRender::FindQueueFamilies(VkPhysicalDevice _VkPhysicalDevice)
	{
		QueueFamilyIndices indices;

		//Get the properties count
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_VkPhysicalDevice, &queueFamilyCount, nullptr);

		//Get the properties data
		std::vector<VkQueueFamilyProperties> queueFamilys(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_VkPhysicalDevice, &queueFamilyCount, queueFamilys.data());

		//Find the graphics bit queue family
		int i = 0; 
		VkBool32 presentSupport = false;
		for (const auto& queueFamily : queueFamilys)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.GraphicsFamily = i;
			}

			vkGetPhysicalDeviceSurfaceSupportKHR(_VkPhysicalDevice, i, _VkSurface, &presentSupport);

			if (presentSupport)
			{
				indices.PresentFamily = i;
			}

			if (indices.isComplete()) break;
			i++;
		}

		return indices;
	}

	KE::RENDERER::QueueFamilyIndices KRender::GetQueueFamilyIndices(VkPhysicalDevice _VkPhysicalDevice)
	{
		QueueFamilyIndices indices;
		return indices;
	}

	KE::KReturn KE::RENDERER::KRender::CreateLogicalDevice(VkPhysicalDevice _VkPhysicalDevice, VkDevice& _VkDevice)
	{
		//Ranges between 0.0 - 1.0
		float QueuePriority = 1.0f;

		std::vector<const char*> extentions = GetRequiredInstanceExtentions();
		QueueFamilyIndices indices = FindQueueFamilies(_VkPhysicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo DeviceQueueInfo{};
			DeviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			DeviceQueueInfo.pNext = VK_NULL_HANDLE;
			DeviceQueueInfo.queueCount = 1;
			DeviceQueueInfo.queueFamilyIndex = queueFamily;
			DeviceQueueInfo.pQueuePriorities = &QueuePriority;
			queueCreateInfos.push_back(DeviceQueueInfo);
		}


		VkPhysicalDeviceFeatures DeviceFeaturesInfo{};

		VkDeviceCreateInfo DeviceInfo{};
		DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		DeviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		DeviceInfo.pQueueCreateInfos = queueCreateInfos.data();
		DeviceInfo.pEnabledFeatures = &DeviceFeaturesInfo;
		DeviceInfo.enabledExtensionCount = 0;
		DeviceInfo.ppEnabledExtensionNames = VK_NULL_HANDLE;

		if (enableValidationLayers)
		{
			DeviceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			DeviceInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			DeviceInfo.enabledLayerCount = 0;
		}

		VkResult result = vkCreateDevice(_VkPhysicalDevice, &DeviceInfo, nullptr, &_VkDevice);

		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create logical device!");
		}

		vkGetDeviceQueue(_VkDevice, indices.GraphicsFamily.value(), 0, &_VkGraphicsQueue);
		vkGetDeviceQueue(_VkDevice, indices.GraphicsFamily.value(), 0, &_VkPresentationQueue);


		return KE::KReturn::K_LOGICAL_DEVICE_CREATION_SUCCESS;
	}
	
	

	bool KRender::IsDeviceSuitable(VkPhysicalDevice _VkPhyscialDevice)
	{
		QueueFamilyIndices Indices = KRender::FindQueueFamilies(_VkPhyscialDevice);

		

		return Indices.isComplete();
	}

	

	std::vector<const char*> KRender::GetRequiredInstanceExtentions()
	{
		std::vector<const char*> extentions;

		if (enableValidationLayers)
		{
			extentions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		extentions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
		extentions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
		extentions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		return extentions;
	}

	std::vector<const char*> KRender::GetRequiredInstaceLayers()
	{
		std::vector<const char*> layers;
		
		if (enableValidationLayers)
		{
			layers.push_back("VK_LAYER_KHRONOS_validation");
		}
		return layers;
	}

}
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
		KE::RENDERER::KRender::PickPhysicalDevice(_VkPhysicalDevice, _VkInstance);
		KE::RENDERER::KRender::CreateLogicalDevice(_VkPhysicalDevice, _VkDevice);
		KE::RENDERER::KRender::CreateSwapChain(_VkPhysicalDevice, _VkDevice, _VkSwapChain);

		return KE::KReturn::K_SUCCESS;
	}

	void KRender::UpdateLoop()
	{

	}

	void KRender::CleanUp()
	{

		vkDestroySurfaceKHR(_VkInstance, _VkSurface, nullptr);
		vkDestroySwapchainKHR(_VkDevice, _VkSwapChain, nullptr);
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

		InstanceExtensions = GetRequiredInstanceExtensions();

		InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		InstanceInfo.pApplicationInfo = &AppInfo;
		InstanceInfo.pNext = VK_NULL_HANDLE;
		InstanceInfo.enabledExtensionCount = static_cast<uint32_t>(InstanceExtensions.size());
		InstanceInfo.ppEnabledExtensionNames = InstanceExtensions.data();

		if (enableValidationLayers)
		{
			throw std::runtime_error("Validation layers requested, but not available");
		}
		else
		{
			InstanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			InstanceInfo.ppEnabledLayerNames = validationLayers.data();
		}

		VkResult result = vkCreateInstance(&InstanceInfo, nullptr, &_VkInstance);
		
		if (vkCreateInstance(&InstanceInfo, nullptr, &_VkInstance) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create VkInstance");
		}

		return KE::KReturn::K_SUCCESS;
	}

	KE::KReturn KRender::CreateWin32Surface(KE::SYSTEM::KWindow& _win, VkInstance _VkInstance, VkSurfaceKHR& _VkSurfaceKHR)
	{

		VkWin32SurfaceCreateInfoKHR _WinSurfaceInfo{};

		_WinSurfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		_WinSurfaceInfo.hwnd = _win.GetWindowHandle();
		_WinSurfaceInfo.hinstance = _win.GetWindowInstance();

		if (vkCreateWin32SurfaceKHR(_VkInstance, &_WinSurfaceInfo, nullptr, &_VkSurfaceKHR) != VK_SUCCESS)
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
		std::vector<VkPhysicalDevice> PhysicalDevices(deviceCount);
		vkEnumeratePhysicalDevices(_VkInstance, &deviceCount, PhysicalDevices.data());

		//Find a suitable device with vulkan support
		for (const auto PhysicalDevice : PhysicalDevices)
		{
			if (IsDeviceSuitable(PhysicalDevice))
			{
				_VkPhysicalDevice = PhysicalDevice;
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
		QueueFamilyIndices indices = FindQueueFamilies(_VkPhysicalDevice);
		return indices;
	}

	KE::RENDERER::SwapChainSupportDetails KRender::GetSwapChainDetails(VkPhysicalDevice _VkPhysicalDevice)
	{
		SwapChainSupportDetails SwapChainDetails;

		//Surface Capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_VkPhysicalDevice, _VkSurface, &SwapChainDetails.SurfaceCapabilities);

		//Format count
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(_VkPhysicalDevice, _VkSurface, &formatCount, nullptr);

		if (formatCount != 0)
		{
			SwapChainDetails.ImageFormats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(_VkPhysicalDevice, _VkSurface, &formatCount, SwapChainDetails.ImageFormats.data());
		}

		//Presentation modes
		uint32_t presentCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(_VkPhysicalDevice, _VkSurface, &presentCount, nullptr);

		if (presentCount != 0)
		{
			SwapChainDetails.PresentMode.resize(presentCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(_VkPhysicalDevice, _VkSurface, &presentCount, SwapChainDetails.PresentMode.data());
		}

		return SwapChainDetails;
	}

	VkSurfaceFormatKHR KRender::ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR> formats)
	{
		for (const auto availableFormat : formats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB //the format encoding
				&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) // applys support for images in SRGB color space
			{
				return availableFormat;
			}

			return formats[0];
		}
	}

	VkPresentModeKHR KRender::ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& presentModes)
	{
		for (const auto& availablePresentMode : presentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return availablePresentMode;
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D KRender::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities)
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
		{
			return capabilities.currentExtent;
		}
		else 
		{
			int width, height;
			_win.GetFrameBufferSize(_win.GetWindowHandle(), width, height);

			VkExtent2D actualExtent =
			{
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)

			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;

		}
	}

	KE::KReturn KE::RENDERER::KRender::CreateLogicalDevice(VkPhysicalDevice _VkPhysicalDevice, VkDevice& _VkDevice)
	{
		//Ranges between 0.0 - 1.0
		float QueuePriority = 1.0f;

		std::vector<const char*> extentions = GetRequiredInstanceExtensions();
		QueueFamilyIndices indices = GetQueueFamilyIndices(_VkPhysicalDevice);

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

		deviceExtensions = GetRequiredDeviceExtensions();

		VkDeviceCreateInfo DeviceInfo{};
		DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		DeviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		DeviceInfo.pQueueCreateInfos = queueCreateInfos.data();
		DeviceInfo.pEnabledFeatures = &DeviceFeaturesInfo;
		DeviceInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		DeviceInfo.ppEnabledExtensionNames = deviceExtensions.data();

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

	KE::KReturn KRender::CreateSwapChain(VkPhysicalDevice _VkPhysicalDevice, VkDevice _VkDevice, VkSwapchainKHR& _VkSwapChain)
	{
		SwapChainSupportDetails SwapChainDetails = GetSwapChainDetails(_VkPhysicalDevice);

		VkSurfaceFormatKHR SurfaceFormat = ChooseSwapChainFormat(SwapChainDetails.ImageFormats);
		VkPresentModeKHR PresentMode = ChooseSwapChainPresentMode(SwapChainDetails.PresentMode);
		VkExtent2D Extent = ChooseSwapExtent(SwapChainDetails.SurfaceCapabilities);
		
		uint32_t ImageCount = SwapChainDetails.SurfaceCapabilities.minImageCount + 1;

		if (SwapChainDetails.SurfaceCapabilities.minImageCount > 0 && ImageCount > SwapChainDetails.SurfaceCapabilities.maxImageCount)
		{
			ImageCount = SwapChainDetails.SurfaceCapabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR SwapChainInfo{};
		SwapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		SwapChainInfo.surface = _VkSurface;
		SwapChainInfo.minImageCount = ImageCount;
		SwapChainInfo.imageFormat = SurfaceFormat.format;
		SwapChainInfo.imageColorSpace = SurfaceFormat.colorSpace;
		SwapChainInfo.imageExtent = Extent;
		SwapChainInfo.imageArrayLayers = 1;
		SwapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices Indices = FindQueueFamilies(_VkPhysicalDevice);
		uint32_t queueFamilyIndices[] = { Indices.GraphicsFamily.value(), Indices.PresentFamily.value() };

		if (Indices.GraphicsFamily != Indices.PresentFamily)
		{
			SwapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Image can be used by muiltple queues
			SwapChainInfo.queueFamilyIndexCount = 2;
			SwapChainInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			SwapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Images can only be used by a single queue
			SwapChainInfo.queueFamilyIndexCount = 0; 
			SwapChainInfo.pQueueFamilyIndices = nullptr; 
		}

		SwapChainInfo.preTransform = SwapChainDetails.SurfaceCapabilities.currentTransform;
		SwapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // ALPHA should not be used for blending windows
		SwapChainInfo.presentMode = PresentMode;
		SwapChainInfo.clipped = VK_TRUE; // Dont care about covered pixels
		SwapChainInfo.oldSwapchain = VK_NULL_HANDLE;
		
		VkResult result = vkCreateSwapchainKHR(_VkDevice, &SwapChainInfo, nullptr, &_VkSwapChain);

		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create SwapChain");
			return KE::KReturn::K_FAILURE;
		}

		vkGetSwapchainImagesKHR(_VkDevice, _VkSwapChain, &ImageCount, nullptr);
		swapChainImages.resize(ImageCount);
		vkGetSwapchainImagesKHR(_VkDevice, _VkSwapChain, &ImageCount, swapChainImages.data());

		_VkSwapChainFormat = SurfaceFormat.format;
		_VkSwapChainExtent = Extent;

		return KE::KReturn::K_SUCCESS;
	}

	KE::KReturn KRender::CreateImageViews()
	{
		ImageViews.resize(swapChainImages.size());
		return KE::KReturn::K_SUCCESS;
	}
	
	

	bool KRender::IsDeviceSuitable(VkPhysicalDevice _VkPhyscialDevice)
	{
		QueueFamilyIndices Indices = KRender::FindQueueFamilies(_VkPhyscialDevice);

		

		return Indices.isComplete();
	}

	std::vector<const char*> KRender::GetRequiredInstanceExtensions()
	{
		std::vector<const char*> extensions;

		if (enableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
		extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

		return extensions;
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

	std::vector<const char*> KRender::GetRequiredDeviceExtensions()
	{
		std::vector<const char*> extensions;
		extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		return extensions;
	}

	bool KRender::IsDeviceSuitable(VkPhysicalDevice _VkPhyscialDevice)
	{
		QueueFamilyIndices Indices = KRender::FindQueueFamilies(_VkPhyscialDevice);

		bool extensionsSupported = CheckDeviceExtensionSupport(_VkPhyscialDevice);

		//Is the SwapChain supported
		bool SwapChainAdequate = false;
		if (extensionsSupported)
		{
			SwapChainSupportDetails SwapChainSupportDetails = GetSwapChainDetails(_VkPhyscialDevice);
			SwapChainAdequate = !SwapChainSupportDetails.ImageFormats.empty() && !SwapChainSupportDetails.PresentMode.empty();
		}

		return Indices.isComplete() && SwapChainAdequate && extensionsSupported;
	}

	bool KRender::CheckDeviceExtensionSupport(VkPhysicalDevice _VkPhysicalDevice)
	{
		uint32_t extensionCount;

		vkEnumerateDeviceExtensionProperties(_VkPhysicalDevice, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(_VkPhysicalDevice, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}
		return requiredExtensions.empty();
	}
}
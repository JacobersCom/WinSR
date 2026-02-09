#include "vkHelper.h"

namespace KE::VULKAN
{

	
	//Creates logical device to interface with the different types of queue familys found on the PhysicalDevice
	KE::KReturn CreateLogicalDevice(VkPhysicalDevice _VkPhysicalDevice, VkDevice& _VkDevice, VkQueue _VkQueue)
	{
		//Ranges between 0.0 - 1.0
		float QueuePriority = 1.0f;
		std::vector<const char*> extentions = GetRequiredInstanceExtentions();
		QueueFamilyIndices indices = FindQueueFamilies(_VkPhysicalDevice);

		VkDeviceQueueCreateInfo DeviceQueueInfo{};
		DeviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		DeviceQueueInfo.pNext = VK_NULL_HANDLE;
		DeviceQueueInfo.queueCount = 1;
		DeviceQueueInfo.queueFamilyIndex = indices.GraphicsFamily.has_value();
		DeviceQueueInfo.pQueuePriorities = &QueuePriority; 

		VkPhysicalDeviceFeatures DeviceFeaturesInfo{};

		VkDeviceCreateInfo DeviceInfo{};
		DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		DeviceInfo.queueCreateInfoCount = 1;
		DeviceInfo.pQueueCreateInfos = &DeviceQueueInfo;
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

		if(result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create logical device!");
		}

		vkGetDeviceQueue(_VkDevice, indices.GraphicsFamily.value(), 0, &_VkQueue);

		return KE::KReturn::K_LOGICAL_DEVICE_CREATION_SUCCESS;
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

static std::vector<const char*> GetRequiredInstanceExtentions()
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
	QueueFamilyIndices Indices = KE::VULKAN::FindQueueFamilies(_VkPhysicalDevice);

	return Indices.isComplete();
}

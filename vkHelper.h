#pragma once


#define VK_USE_PLATFORM_WIN32_KHR

#include <iostream>
#include <vector>
#include <optional>

#include "KReturn.h"
#include "vulkan/vulkan.h"

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else 
	const bool enableValidationLayers = true;
#endif

static struct QueueFamilyIndices {

	std::optional<uint32_t>(GraphicsFamily);
	std::optional<uint32_t>(PresentFamily);
	

	bool isComplete()
	{
		return GraphicsFamily.has_value();

	}
};

static std::vector<const char*> validationLayers = {

	"VK_LAYER_KHRONOS_validation"
};

//Vulkan helper functions
namespace KE::VULKAN
{


	//Creates a Vulkan Surface for win32

	//Picks a GPU with Vulkan driver support
	
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _VkPhysicalDevice);

	QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice _VkPhysicalDevice);

	KE::KReturn CreateLogicalDevice(VkPhysicalDevice _VkPhysicalDevice, VkDevice& _VkDevice, VkQueue _VkQueue);
	
	void SetUpDebuggerMessage();
}

//May need more error checking
static bool CheckValidationLayerSupport();
static std::vector<const char*> GetRequiredInstanceExtentions();
static bool IsDeviceSuitable(VkPhysicalDevice _VkPhysicalDevice);
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT _MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT _MessageType,
	const VkDebugUtilsMessengerCallbackDataEXT* _pCallBackData, void* _pUserData);


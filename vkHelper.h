#include "KReturn.h"
#include "vulkan/vulkan.h"

KE::KReturn CreateVkInstance(VkInstance _VkInstance)
{
	VkApplicationInfo AppInfo{};

	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.pNext = VK_NULL_HANDLE;
	AppInfo.pApplicationName = "KOS Engine";
	AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.pEngineName = "KOS";
	AppInfo.apiVersion = VK_API_VERSION_1_4;

	VkInstanceCreateInfo InstanceInfo{};

	InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceInfo.pApplicationInfo = &AppInfo;
	InstanceInfo.pNext = VK_NULL_HANDLE;
}



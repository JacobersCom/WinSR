#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#include "KReturn.h"
#include "vulkan/vulkan.h"


//Creates a Instance of the vulkan API
KE::KReturn CreateVkInstance(VkInstance* _VkInstance);



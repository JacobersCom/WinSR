#pragma once
  
#define VK_USE_PLATFORM_WIN32_KHR

#include <iostream>
#include <vector>

#include "KReturn.h"
#include "vulkan/vulkan.h"

std::vector<const char*> validationLayers = {

	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else 
	const bool enableValidationLayers = true;
#endif

//Creates a Instance of the vulkan API
KE::KReturn CreateVkInstance(VkInstance& _VkInstance);

//Creates a Vulkan Surface for win32
KE::KReturn CreateWin32Surface(HWND _windowHandle, HINSTANCE _windowInstance, VkInstance _VkInstance);

bool CheckValidationLayerSupport();

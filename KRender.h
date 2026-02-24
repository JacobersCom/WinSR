#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#include <iostream>
#include <vector>
#include <optional>
#include <set>

#include "vulkan/vulkan.h"
#include "KWindow.h"
#include "KReturn.h"

#ifdef NDEBUG
static const bool enableValidationLayers = false;
#else 
static const bool enableValidationLayers = true;
#endif

namespace KE::RENDERER
{

	struct QueueFamilyIndices {

		std::optional<uint32_t>(GraphicsFamily);
		std::optional<uint32_t>(PresentFamily);


		bool isComplete()
		{
			return GraphicsFamily.has_value() && PresentFamily.has_value();
		}
	};

	class KRender
	{
	public:

		KRender(KE::SYSTEM::KWindow& _win) : _win(_win){}

		void run();

		const VkInstance GetVkInstance() { return _VkInstance; }
		
	private:

		KE::KReturn CreateVkInstance(VkInstance& _Instance);
		KE::KReturn PickPhysicalDevice(VkPhysicalDevice& _VkPhysicalDevice, VkInstance _VkInstance);
		KE::KReturn CreateWin32Surface(KE::SYSTEM::KWindow& _win, VkInstance _VkInstance, VkSurfaceKHR& _VkSurfaceKHR);
		KE::RENDERER::QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _VkPhysicalDevice);
		KE::RENDERER::QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice _VkPhysicalDevice);
		KE::KReturn CreateLogicalDevice(VkPhysicalDevice _VkPhysicalDevice, VkDevice& _VkDevice);
		KE::KReturn InitVulkan();
		
		void UpdateLoop();
		void CleanUp();

		std::vector<const char*> GetRequiredInstanceExtentions();
		std::vector<const char*> GetRequiredInstaceLayers();
		bool IsDeviceSuitable(VkPhysicalDevice _VkPhyscialDevice);
		

	private:

		KE::SYSTEM::KWindow& _win;
		VkInstance _VkInstance;
		VkDevice _VkDevice;
		VkQueue _VkGraphicsQueue;
		VkQueue _VkPresentationQueue;
		VkSurfaceKHR _VkSurface;
		VkPhysicalDevice _VkPhyscialDevice;

		std::vector<const char*> validationLayers;
		std::vector<const char*> deviceExtensions;
	};
}
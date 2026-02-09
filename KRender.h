#pragma once

#include <optional>

#include "vkHelper.h"
#include "KWindow.h" 

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
			return GraphicsFamily.has_value();

		}
	};

	class KRender
	{
	public:

		void run();

		const VkInstance GetVkInstance() { return _VkInstance; }

	private:

		KE::KReturn CreateVkInstance(VkInstance& _Instance);
		KE::KReturn CreateWin32Surface(HWND _windowHandle, HINSTANCE _windowInstance, 
			VkInstance _VkInstance, VkSurfaceKHR& _VkSurfaceKHR);
		KE::KReturn PickPhysicalDevice(VkPhysicalDevice& _VkPhysicalDevice, VkInstance _VkInstance);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _VkPhysicalDevice);
		QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice _VkPhysicalDevice);
		KE::KReturn CreateLogicalDevice(VkPhysicalDevice _VkPhysicalDevice, VkDevice& _VkDevice, VkQueue _VkQueue);
		KE::KReturn InitVulkan();
		void UpdateLoop();
		void CleanUp();


	private:

		KE::SYSTEM::KWindow _win;
		VkInstance _VkInstance;
		VkDevice _VkDevice;
		VkQueue _VkQueue;
		VkSurfaceKHR _VkSurface;
		VkPhysicalDevice _VkPhyscialDevice;

		static std::vector<const char*> validationLayers;
	};
}
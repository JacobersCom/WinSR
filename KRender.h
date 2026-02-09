#pragma once

#include "vkHelper.h"
#include "KWindow.h" 

namespace KE::RENDERER
{
	class KRender
	{
	public:

		void run();

		const VkInstance GetVkInstance() { return _VkInstance; }

	private:

		KE::KReturn CreateVkInstance(VkInstance& _Instance);
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
	};
}
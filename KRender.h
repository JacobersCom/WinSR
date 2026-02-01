#pragma once

#include "vkHelper.h"
#include "KWindow.h" 

namespace KE::SYSTEM
{
	class KRender
	{
	public:

		void run();

		const VkInstance GetVkInstance() { return _VkInstance; }

	private:

		KE::KReturn InitVulkan();
		void UpdateLoop();
		void CleanUp();


	private:

		KE::SYSTEM::KWindow _win;
		VkInstance _VkInstance;
		VkDevice _VkDevice;
		VkPhysicalDevice _VkPhyscialDevice;
	};
}
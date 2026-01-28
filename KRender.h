#pragma once

#include "vkHelper.h"


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

		VkInstance _VkInstance;
	};
}
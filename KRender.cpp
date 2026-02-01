#include "KRender.h"


namespace KE::SYSTEM
{
	void KRender::run()
	{
		InitVulkan();
		UpdateLoop();
		CleanUp();
	}

	KReturn KRender::InitVulkan() 
	{
		KE::VULKAN::CreateVkInstance(_VkInstance);
		return KE::KReturn::K_SUCCESS;
	}

	void KRender::UpdateLoop()
	{

	}

	void KRender::CleanUp()
	{

	}

}
#pragma once
#include <vulkan/vulkan.h>

namespace Orchis {

	class ImageSamplerManager
	{
	public:
		ImageSamplerManager() = delete;
		ImageSamplerManager(const ImageSamplerManager&) = delete;
		ImageSamplerManager(ImageSamplerManager&&) = delete;
		~ImageSamplerManager() = delete;

		static VkSampler GetSampler() { return s_ImageSampler; }

		static void Init();
		static void Shutdown();

	private:
		static VkSampler s_ImageSampler;
	};
}
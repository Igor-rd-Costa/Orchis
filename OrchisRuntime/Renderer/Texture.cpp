#include "Texture.h"
#include "Platform/Vulkan/VulkanTexture.h"

namespace Orchis {
    Texture::Texture(uint32_t index)
        : m_Index(index) {}

    Texture* Texture::Create(const std::string& path, uint32_t index)
    {
        return new VulkanTexture(path, index);
    }

    Texture* Texture::Create(const std::vector<char>& bytes, uint32_t width, uint32_t height, uint32_t channelCount, uint32_t index)
    {
        return new VulkanTexture(bytes, width, height, channelCount, index);
    }

}
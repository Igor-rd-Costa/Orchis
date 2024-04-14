#include "Texture.h"
#include "Platform/Vulkan/VulkanTexture.h"

namespace Orchis {

    Texture* Texture::Create(const std::string& path, uint32_t index)
    {
        return new VulkanTexture(path, index);
    }

}
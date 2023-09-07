#include "OrchisPCH.h"
#include "Texture.h"
#include "Platform/Vulkan/VulkanTexture.h"

namespace Orchis {

    Ref<Texture> Texture::Create(const char* path, uint32_t binding, ShaderStageFlags stageFlags)
    {
        return CreateRef<VulkanTexture>(path, binding, stageFlags);
    }

}
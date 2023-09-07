#pragma once
#include <stdint.h>

namespace Orchis {

enum ShaderStageFlagBits
{
	SHADER_STAGE_VERTEX = 0x00000001,
	SHADER_STAGE_FRAGMENT = 0x00000010,
	SHADER_STAGE_ALL = 0x7FFFFFFF
};

typedef uint32_t ShaderStageFlags;

}
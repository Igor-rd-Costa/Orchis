#pragma once
#include "Macros.h"
#include "Renderer/Flags.h"

namespace Orchis {

	class Texture
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;

		virtual uint32_t GetBinding() const = 0;
		virtual void Load(const char* path) = 0;

		static Ref<Texture> Create(const char* path, uint32_t binding, ShaderStageFlags stageFlags);
	};
}
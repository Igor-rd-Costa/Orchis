#pragma once
#include "Macros.h"
#include "Renderer/Flags.h"

namespace Orchis {

	class Texture
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;

		virtual void Load(const char* path) = 0;
		virtual void Bind() const = 0;

		static Texture* Create(const std::string& path, uint32_t index);
	};
}
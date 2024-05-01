#pragma once
#include "Macros.h"
#include "Renderer/Flags.h"

namespace Orchis {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Load(const char* path) = 0;
		virtual void Bind() const = 0;

		static Texture* Create(const std::string& path, uint32_t index);
		static Texture* Create(const std::vector<char>& bytes, uint32_t width, uint32_t height, uint32_t channelCount, uint32_t index);
	protected:
		Texture(uint32_t index);
		uint32_t m_Index;
	};
}
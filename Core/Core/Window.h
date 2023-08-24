#pragma once
#include "Macros.h"
#include  <string>
namespace Orchis {

	class ORCHIS_API Window
	{
	public:
		virtual ~Window() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Update() = 0;

		static Ref<Window> Create(std::string_view name);

	private:
	};
}
#pragma once
#include "Macros.h"
#include "Input.h"
#include  <string>
namespace Orchis {

	class ORCHIS_API Window
	{
	public:
		virtual ~Window() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void Update() = 0;
		virtual std::pair<long, long> GetCenter() const = 0;

		virtual void* GetHandle() = 0;
		static Scope<Window> Create(std::string_view name);
	protected:
		void UpdateInputState() const { Input::UpdateState(); }
	private:
		friend class VulkanAPI;
		//definition located in plaform specific files
		static const char** GetRequiredVKExtensions(uint32_t* outExtensionCount);
	};
}
#pragma once
#include "Macros.h"
#include "Input.h"
#include  <string>
namespace Orchis {

	class __declspec(dllexport) Window
	{
	public:
		Window() = default;
		virtual ~Window() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void Update() = 0;
		virtual std::pair<long, long> GetCenter() const = 0;

		virtual void* GetHandle() = 0;
		static Window* Create(void* parentHandle = nullptr);
	protected:
		void UpdateInputState() const { Input::UpdateState(); }
	private:
		friend class VulkanAPI;
		//definition located in plaform specific files
		static const char** GetRequiredVKExtensions(uint32_t* outExtensionCount);
	};
}
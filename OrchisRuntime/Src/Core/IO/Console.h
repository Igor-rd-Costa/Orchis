#pragma once
#include "OutputBase.h"

namespace Orchis {

	class OC_API Console : public OutputBase
	{
	public:
		static void Open();
		static void Close();
		static WeakRef<Console> Instance();

		static void WriteLine(std::string_view msg)
		{
			if (s_Instance)
			{
				s_Instance->WriteLineImpl(msg);
			}
		}

		template<typename... Args>
		static void WriteLine(const std::string_view msg, Args&&... args)
		{
			if (s_Instance)
			{
				s_Instance->WriteLineImpl(msg, std::forward<Args>(args)...);
			}
		}

	protected:
		static inline Console* s_Instance = nullptr;
		Console() = default;
		virtual ~Console() = default;
		Console(const Console&) = delete;
		Console(Console&&) = delete;
	};
}
#pragma once
#include <Windows.h>
#include "Core/IO/Cursor.h"
#include "Core/IO/Console.h"

namespace Orchis {

	inline HINSTANCE WindowsAppInstance()
	{
		return (HINSTANCE)GetModuleHandle(NULL);
	}

	class WindowsCursor : public Cursor
	{
	public:
		WindowsCursor() = default;
		~WindowsCursor() = default;
	protected:
		virtual void SetPositionImpl(const glm::i64vec2& position) override;
		virtual glm::i64vec2 GetPositionImpl() override;
	};

	class WindowsConsole : public Console
	{
	public:
		WindowsConsole();
		~WindowsConsole();

	protected:
		virtual void WriteLineImpl(std::string_view msg) const override;
		virtual void WriteImpl(std::string_view msg) const override;
	private:
		HANDLE m_StdInputHandle;
		HANDLE m_StdOutputHandle;
		HANDLE m_StdErrorHandle;
	};
}
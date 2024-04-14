#pragma once
#include <string>
#include <Windows.h>
#include "KeyCodes.h"
#include <bitset>

namespace Orchis {

	std::wstring StringToWideString(const std::string& string);

	std::wstring StringToWideString(std::string_view string);

	Key TranslateKeyCode(WPARAM wParam, LPARAM lParam);
}
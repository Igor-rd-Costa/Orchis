#include "OrchisWindows.h"
#include "Core/IO/Logger.h"

namespace Orchis {

	void WindowsCursor::SetPositionImpl(const glm::i64vec2& position)
	{
		if (SetCursorPos(static_cast<int>(position.x), static_cast<int>(position.y)) == false)
		{
			DWORD error = GetLastError();
			OC_LOG_ERROR("WindowsCursor Error: SetCursorPos => {}", error);
			OC_ASSERT(false);
		}
	}

	glm::i64vec2 WindowsCursor::GetPositionImpl()
	{
		static POINT point;
		GetCursorPos(&point);
		return { point.x, point.y };
	}
}
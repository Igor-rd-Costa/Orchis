#include "Cursor.h"

namespace Orchis {

	Cursor::Cursor()
	{
		OC_ASSERT((s_Instance == nullptr));
		s_Instance = this;
	}

	glm::i64vec2 Cursor::GetPosition()
	{
		return s_Instance->GetPositionImpl();
	}

	void Cursor::SetPosition(const glm::i64vec2& position)
	{
		s_Instance->SetPositionImpl(position);
	}
}
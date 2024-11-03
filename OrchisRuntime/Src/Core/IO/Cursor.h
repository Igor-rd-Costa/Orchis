#pragma once
#include <glm/glm.hpp>
#include "../Src/Core/Macros.h"

namespace Orchis {

	class OC_API Cursor
	{
	public:
		static glm::i64vec2 GetPosition();
		static void SetPosition(const glm::i64vec2& position);

	protected:
		virtual void SetPositionImpl(const glm::i64vec2& pos) = 0;
		virtual glm::i64vec2 GetPositionImpl() = 0;
		Cursor();
		~Cursor() = default;
	private:
		static inline Cursor* s_Instance = nullptr;
		Cursor(const Cursor&) = delete;
		Cursor(Cursor&&) = delete;
	};

}
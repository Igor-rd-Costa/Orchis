#pragma once
#include "Macros.h"

namespace Orchis
{
	class OC_API UUID
	{
	public:
		~UUID() = default;

		const char* ToString() const;
		bool IsNull() const { return (m_Uuid[0] == 0 && m_Uuid[1] == 0); }
		bool operator==(const UUID&) const;

		operator bool() const { return !IsNull(); }
		operator const char* () const { return ToString(); }

		static UUID Null() { return UUID(0, 0); };
		static UUID Generate();
	private:
		UUID();
		UUID(uint64_t a, uint64_t b);
		uint64_t m_Uuid[2];
	};
}
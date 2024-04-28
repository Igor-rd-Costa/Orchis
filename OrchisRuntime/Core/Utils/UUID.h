#pragma once
#include "Macros.h"

namespace Orchis
{
	class OC_API UUID
	{
	public:
		UUID();
		~UUID() = default;

		const char* ToString() const;
		bool IsNull() const { return (m_Uuid[0] == 0 && m_Uuid[1] == 0); }
		bool operator==(const UUID&) const;

		operator bool() const { return !IsNull(); }
		operator const char* () const { return ToString(); }

		static UUID Null() { return UUID(0, 0); };
		static UUID Generate();
		static UUID FromString(std::string_view uuid);
		static UUID FromString(const std::string& uuid);
		uint64_t GetFirst() const
		{
			return m_Uuid[0];
		}
		uint64_t GetSecond() const
		{
			return m_Uuid[1];
		}
	private:
		friend ::std::hash<UUID>;
		UUID(uint64_t a, uint64_t b);

		uint64_t m_Uuid[2];
	};
}

template<>
struct std::hash<Orchis::UUID>
{
	std::size_t operator()(const Orchis::UUID& k) const
	{
		std::size_t res = 17;
		res = res * 31 + hash<uint64_t>()(k.GetFirst());
		res = res * 31 + hash<uint64_t>()(k.GetSecond());
		return res;
	}
};
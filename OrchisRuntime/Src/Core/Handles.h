#pragma once
#include <string_view>
#include <vector>
#include <functional>
#include <cstdint>
#include "../Src/Core/Macros.h"

namespace Orchis {

	class OName
	{
	public:
		OName(std::string_view name);
		OName(size_t handle);

		bool operator== (const OName& other)
		{
			return m_Handle == other.m_Handle;
		}
	private:
		size_t m_Handle;
	};

	class OHandle
	{
	public:
		//TODO: Implement this
		OHandle();
		OHandle(const OHandle&) = default;
		OHandle(OHandle&&) = default;
		~OHandle() = default;

		static const OHandle& Null() 
		{ 
			static OHandle nullHandle = OHandle(0);
			return nullHandle; 
		}

		OHandle& operator=(const OHandle& other) = default;
		OHandle& operator=(OHandle&& other) = default;
		bool operator==(const OHandle& other) const
		{
			return m_Handle == other.m_Handle;
		}
	private:
		OHandle(uint64_t val) : m_Handle(val) {}
		friend struct ::std::hash<OHandle>;
		uint64_t m_Handle = 0;
	};

}

inline std::hash<uint64_t> uint64Hasher;
namespace std {

	template<>
	struct hash<::Orchis::OHandle> {
		size_t operator()(const ::Orchis::OHandle& handle) const noexcept
		{
			return uint64Hasher(handle.m_Handle);
		}

		hash() noexcept = default;
		hash(const hash&) noexcept = default;
		hash(hash&&) noexcept = default;
		hash& operator=(const hash&) noexcept = default;
		hash& operator=(hash&&) noexcept = default;
	};

	template<>
	struct equal_to<::Orchis::OHandle> {
		bool operator()(const ::Orchis::OHandle& handle, const ::Orchis::OHandle& other) const noexcept
		{
			return handle == other;
		}

		equal_to() noexcept = default;
		equal_to(const equal_to&) noexcept = default;
		equal_to(equal_to&&) noexcept = default;
		equal_to& operator=(const equal_to&) noexcept = default;
		equal_to& operator=(equal_to&&) noexcept = default;
	};
}
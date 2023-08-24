#pragma once
#include <memory>

#ifdef ORCHIS_PLATFORM_WINDOWS
	#ifdef ORCHIS_BUILD_DYNAMIC
		#define ORCHIS_API //__declspec(dllexport)
	#else
		#define ORCHIS_API //__declspec(dllimport)
	#endif
#else
#error "Orchis only supports Windows."
#endif

namespace Orchis {

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args) 
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

}
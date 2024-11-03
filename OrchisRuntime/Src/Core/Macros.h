#pragma once

namespace Orchis {

#ifdef OC_WINDOWS
#define OC_DEBUG_BREAK __debugbreak()
#else // !OC_WINDOWS
#define OC_DEBUG_BREAK
#endif // OC_WINDOWS

#ifdef OC_DEBUG_BUILD
#define OC_ASSERT(x) if(!x)OC_DEBUG_BREAK
#else // !OC_DEBUG_BUILD
#define OC_ASSERT(x)
#endif // OC_DEBUG_BUILD

#ifdef OC_SHARED_BUILD
#ifdef OC_WINDOWS
#define OC_API
#endif // OC_WINDOWS
#else
#ifdef OC_WINDOWS
#define OC_API
#endif // OC_WINDOWS
#endif

	template<typename T>
	using WeakRef = T*;

}
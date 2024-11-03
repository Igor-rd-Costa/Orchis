#include "Handles.h"
#include <random>

namespace Orchis {
	static std::hash<std::string_view> hasher;
	static std::random_device rdm_d;
	static std::mt19937 random(rdm_d());

	OName::OName(std::string_view name)
		: m_Handle(hasher(name)) {}

	OName::OName(size_t handle)
		: m_Handle(handle) {}



	OHandle::OHandle()
		: m_Handle(random()) {}
}
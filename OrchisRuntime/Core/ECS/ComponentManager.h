#pragma once
#include "Macros.h"
#include "Utils/UUID.h"
#include "Components.h"
#include "glm/glm.hpp"
#include <vector>

namespace Orchis
{
	enum class OC_API ComponentType
		: char
	{
		TRANSFORM, TEXTURE, MESH
	};

	struct OC_API Component
	{
		ComponentType type;
		UUID id;
	};

	class OC_API ComponentManager
	{
	public:
		ComponentManager() = delete;
		ComponentManager(const ComponentManager&) = delete;
		ComponentManager(ComponentManager&&) = delete;

		static Component CreateTransformComponent();
		static Component CreateTransformComponent(const UUID& componentId, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

	private:
		static std::vector<TransformComponent> s_TransformComponents;
	};
}

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
		TRANSFORM, TEXTURE, MESH, INVALID
	};

	struct OC_API Component
	{
		UUID id;
		ComponentType type;
	};

	class OC_API ComponentManager
	{
	public:
		ComponentManager() = delete;
		ComponentManager(const ComponentManager&) = delete;
		ComponentManager(ComponentManager&&) = delete;

		static Component CreateComponent(ComponentType type);
		static Component CreateTransformComponent();
		static Component CreateTransformComponent(const UUID& componentId, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

		static void RemoveComponent(const UUID& componentId, ComponentType type);
		static void RemoveTransformComponent(const UUID& componentId);

		static TransformComponent* GetTranformComponent(const UUID componentId);
	private:
		static std::vector<TransformComponent> s_TransformComponents;
		static std::vector<MeshComponent> s_MeshComponents;
	};
}

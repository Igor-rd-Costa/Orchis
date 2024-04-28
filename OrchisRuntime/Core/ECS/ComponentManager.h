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
		static Component CreateMeshComponent();
		static Component CreateMeshComponent(const UUID& componentId, const UUID& meshId, MeshType meshType);

		static void RemoveComponent(const UUID& componentId, ComponentType type);
		static void RemoveTransformComponent(const UUID& componentId);
		static void RemoveMeshComponent(const UUID& componentId);

		static TransformComponent* GetTranformComponent(const UUID& componentId);
		static MeshComponent* GetMeshComponent(const UUID& componentId);

		static void UpdateTransformComponentPosition(const UUID& componentId, glm::vec3 position);
		static void UpdateTransformComponentRotation(const UUID& componentId, glm::vec3 rotation);
		static void UpdateTransformComponentScale(const UUID& componentId, glm::vec3 scale);

		static void UpdateMeshComponentMeshId(const UUID& componentId, const UUID& meshId);
		static void UpdateMeshComponentMeshType(const UUID& componentId, MeshType meshType);
	private:
		static std::vector<TransformComponent> s_TransformComponents;
		static std::vector<MeshComponent> s_MeshComponents;
	};
}

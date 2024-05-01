#pragma once
#include "Macros.h"
#include "glm/glm.hpp"
#include "Utils/UUID.h"
#include "Mesh.h"

namespace Orchis
{
	struct OC_API TransformComponent
	{
		UUID id;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		TransformComponent()
			: id(UUID::Null()), position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(glm::vec3(0.0f)) {}

		TransformComponent(const UUID& componentId)
			: id(componentId), position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(glm::vec3(0.0f)) {}

		TransformComponent(const UUID& componentId, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
			: id(componentId), position(position), rotation(rotation), scale(scale) {}
	};

	enum MeshType
	{
		STATIC
	};

	struct OC_API MeshComponent
	{
		UUID id;
		UUID meshId;
		MeshType meshType = MeshType::STATIC;
		
		MeshComponent()
			: id(UUID::Null()), meshId(UUID::Null()) {}
		
		MeshComponent(const UUID& componentId, const UUID& meshId, MeshType meshType)
			: id(componentId), meshId(meshId), meshType(meshType) {}
	};

	struct OC_API TextureComponent
	{
		UUID id;
		UUID textureId;

		TextureComponent()
			: id(UUID::Null()), textureId(UUID::Null()) {}

		TextureComponent(const UUID& componentId, const UUID& textureId)
			: id(componentId), textureId(textureId) {}
	};
}
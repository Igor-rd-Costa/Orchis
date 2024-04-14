#pragma once
#include "Macros.h"
#include "glm/glm.hpp"
#include "Utils/UUID.h"

namespace Orchis
{
	struct OC_API TransformComponent
	{
		TransformComponent(const UUID& componentId)
			: id(componentId), position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(glm::vec3(0.0f)) {}

		TransformComponent(const UUID& componentId, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
			: id(componentId), position(position), rotation(rotation), scale(scale) {}

		UUID id;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};
}
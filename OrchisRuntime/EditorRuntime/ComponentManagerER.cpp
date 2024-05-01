#include "Macros.h"
#include "ComponentManager.h"

using namespace Orchis;

OC_EXPORT TransformComponent OrchisComponentManagerGetTransformComponent(const UUID componentId)
{
	TransformComponent* component =	ComponentManager::GetTranformComponent(componentId);
	if (component)
		return *component;

	return TransformComponent();
}

OC_EXPORT MeshComponent OrchisComponentManagerGetMeshComponent(const UUID componentId)
{
	MeshComponent* component = ComponentManager::GetMeshComponent(componentId);
	if (component)
		return *component;

	return MeshComponent();
}

OC_EXPORT TextureComponent OrchisComponentManagerGetTextureComponent(const UUID componentId)
{
	TextureComponent* c = ComponentManager::GetTextureComponent(componentId);
	if (c)
		return *c;
	return TextureComponent();
}


OC_EXPORT void OrchisComponentManagerTransformComponentSetPosition(const UUID componentId, glm::vec3 position)
{
	ComponentManager::UpdateTransformComponentPosition(componentId, position);
}
OC_EXPORT void OrchisComponentManagerTransformComponentSetRotation(const UUID componentId, glm::vec3 rotation)
{
	ComponentManager::UpdateTransformComponentRotation(componentId, rotation);
}
OC_EXPORT void OrchisComponentManagerTransformComponentSetScale(const UUID componentId, glm::vec3 scale)
{
	ComponentManager::UpdateTransformComponentScale(componentId, scale);
}

OC_EXPORT void OrchisComponentManagerUpdateMeshComponentMeshId(const UUID componentId, const UUID meshId)
{
	ComponentManager::UpdateMeshComponentMeshId(componentId, meshId);
}

OC_EXPORT void OrchisComponentManagerUpdateTextureComponentTextureId(const UUID componentId, const UUID newTextureId)
{
	ComponentManager::UpdateTextureComponentTextureId(componentId, newTextureId);
}


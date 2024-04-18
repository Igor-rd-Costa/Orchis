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
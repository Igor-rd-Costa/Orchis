#include "Scene.h"
#include "Timer.h"
#include <array>
namespace Orchis {
	
	Scene::Scene()
		: m_Id(UUID::Generate()) {}

	Scene::Scene(const UUID& id)
		:m_Id(id)
	{
		if (id.IsNull())
		{
			m_Id = UUID::Generate();
		}
	}

	Entity* Scene::AddEntity()
	{
		return &m_Entities.emplace_back();
	}

	Entity* Scene::AddEntity(const UUID& entityId)
	{
		return &m_Entities.emplace_back(entityId);
	}

	void Scene::RemoveEntity(const UUID& entityId)
	{
		for (auto it = m_Entities.begin(); it != m_Entities.end(); it++)
		{
			if (it->Id() == entityId)
			{
				m_Entities.erase(it);
				break;
			}
		}
	}

	Entity* Scene::GetEntity(const UUID& entityId)
	{
		for (Entity& entity : m_Entities)
		{
			if (entity.Id() == entityId)
				return &entity;
		}
		return nullptr;
	}
}
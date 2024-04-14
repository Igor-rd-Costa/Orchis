#include "SceneManager.h"
#include "Macros.h"

namespace Orchis {

	std::vector<Scene*> SceneManager::s_ActiveScenes = {};
	std::vector<Scene> SceneManager::s_Scenes = {};

	Scene* SceneManager::GetScene(const UUID& sceneId)
	{
		for (Scene& scene : s_Scenes)
		{
			if (scene.Id() == sceneId)
			{
				return &scene;
			}
		}

		return nullptr;
	}

	Scene* SceneManager::CreateScene(bool makeActive)
	{
		Scene* scene = &s_Scenes.emplace_back();
		if (makeActive)
		{
			scene->SetActive(true);
			s_ActiveScenes.push_back(scene);
		}
		return scene;
	}

	Scene* SceneManager::CreateScene(const UUID& sceneId, bool makeActive)
	{
		Scene* scene = &s_Scenes.emplace_back(sceneId);
		if (makeActive)
		{
			scene->SetActive(true);
			s_ActiveScenes.push_back(scene);
		}
		return scene;
	}

	void SceneManager::DeleteScene(const UUID& sceneId)
	{
		for (auto it = s_Scenes.begin(); it != s_Scenes.end(); it++)
		{
			if (it->Id() == sceneId)
			{
				if (it->IsActive())
				{
					for (auto itTwo = s_ActiveScenes.begin(); itTwo != s_ActiveScenes.end(); itTwo++)
					{
						if ((*itTwo)->Id() == it->Id())
						{
							s_ActiveScenes.erase(itTwo);
						}
					}
				}
				s_Scenes.erase(it);
			}
		}
	}

	void SceneManager::LoadScene(const UUID& sceneId)
	{
		for (Scene& scene : s_Scenes)
		{
			if (scene.Id() == sceneId)
			{
				scene.SetActive(true);
				s_ActiveScenes.push_back(&scene);
			}
		}
	}

	void SceneManager::UnloadScene(const UUID& sceneId)
	{
		for (auto it = s_ActiveScenes.begin(); it != s_ActiveScenes.end(); it++)
		{
			if ((*it)->Id() == sceneId)
			{
				(*it)->SetActive(false);
				s_ActiveScenes.erase(it);
				break;
			}
		}
	}
}
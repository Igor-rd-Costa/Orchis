#include "SceneManager.h"
#include "Macros.h"

namespace Orchis {

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
			scene->SetActive(true);
		return scene;
	}

	Scene* SceneManager::CreateScene(const UUID& sceneId, bool makeActive)
	{
		Scene* scene = &s_Scenes.emplace_back(sceneId);
		if (makeActive)
			scene->SetActive(true);
		
		return scene;
	}

	void SceneManager::DeleteScene(const UUID& sceneId)
	{
		for (auto it = s_Scenes.begin(); it != s_Scenes.end(); it++)
		{
			if (it->Id() == sceneId)
			{
				s_Scenes.erase(it);
				return;
			}
		}
	}

	void SceneManager::DebugScenes()
	{
		std::cout << "Engine: " << s_Scenes.size() << " scenes.\n";
	}

	void SceneManager::LoadScene(const UUID& sceneId)
	{
		for (Scene& scene : s_Scenes)
		{
			if (scene.Id() == sceneId)
			{
				scene.SetActive(true);
				return;
			}
		}
	}

	void SceneManager::UnloadScene(const UUID& sceneId)
	{
		for (Scene& scene : s_Scenes)
		{
			if (scene.Id() == sceneId)
			{
				scene.SetActive(false);
				return;
			}
		}
	}

	const std::vector<Scene*> SceneManager::GetActiveScenes()
	{
		std::vector<Scene*> scenes = {};
		for (Scene& scene : s_Scenes)
		{
			if (scene.IsActive())
				scenes.push_back(&scene);
		}
		return scenes;
	}
}
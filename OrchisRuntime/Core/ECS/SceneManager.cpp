#include "SceneManager.h"
#include "Macros.h"

namespace Orchis {

	Scene* SceneManager::s_ActiveScene = nullptr;

	Scene* SceneManager::GetScene()
	{
		return s_ActiveScene;
	}

	Scene* SceneManager::CreateScene()
	{
		UnloadScene();
		s_ActiveScene = new Scene();
		return s_ActiveScene;
	}

	Scene* SceneManager::CreateScene(const UUID& sceneId)
	{
		UnloadScene();
		s_ActiveScene = new Scene(sceneId);
		return s_ActiveScene;
	}

	void SceneManager::LoadScene(Scene* scene)
	{
		UnloadScene();
		s_ActiveScene = scene;
	}

	void SceneManager::UnloadScene()
	{
		if (s_ActiveScene)
			delete s_ActiveScene;
	}
}
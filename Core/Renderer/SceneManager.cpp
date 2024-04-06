#include "SceneManager.h"

namespace Orchis {

	Scene* SceneManager::s_Scene;

	void SceneManager::ShutDown()
	{
		if (s_Scene)
			UnloadScene();
	}

	void* SceneManager::CreateScene(std::string_view name)
	{
		if (s_Scene)
			UnloadScene();

		s_Scene = new Scene(name);
		return (void*)s_Scene;
	}

	void SceneManager::UnloadScene()
	{
		delete s_Scene;
		s_Scene = nullptr;
	}
}
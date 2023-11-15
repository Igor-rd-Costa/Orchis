#include "SceneManager.h"

namespace Orchis {

	Scene* SceneManager::s_Scene;

	void SceneManager::ShutDown()
	{
		if (s_Scene)
			UnloadScene();
	}

	void SceneManager::CreateScene(std::string_view name)
	{
		std::cout << "Entered scene creation!\n";
		if (s_Scene)
			UnloadScene();

		s_Scene = new Scene(name);
		std::cout << "Created scene!\n";
	}

	void SceneManager::UnloadScene()
	{
		delete s_Scene;
		s_Scene = nullptr;
	}
}
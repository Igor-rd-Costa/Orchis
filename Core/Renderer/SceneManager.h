#pragma once
#include "Scene.h"

namespace Orchis {

	class SceneManager
	{
	public:
		SceneManager() = delete;
		SceneManager(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;

		static void ShutDown();
		static Scene* GetScene() { return s_Scene; }

		static void CreateScene(std::string_view name);

		//TODO implement this
		//static void LoadScene(std::string_view filePath);
		static void UnloadScene();
	private:
		static Scene* s_Scene;
	};
}
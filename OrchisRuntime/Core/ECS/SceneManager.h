#pragma once
#include "Macros.h"
#include "Scene.h"
#include "Utils/UUID.h"
#include <vector>

namespace Orchis {

	class OC_API SceneManager
	{
	public:
		SceneManager() = delete;
		SceneManager(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;

		static Scene* GetScene();
		static Scene* CreateScene();
		static Scene* CreateScene(const UUID& sceneId);
		static void LoadScene(Scene* scene);
		static void UnloadScene();
	private:
		static Scene* s_ActiveScene;
	};
}
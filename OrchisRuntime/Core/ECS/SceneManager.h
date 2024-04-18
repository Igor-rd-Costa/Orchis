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

		static Scene* GetScene(const UUID& sceneId);
		static const std::vector<Scene*> GetActiveScenes();

		static Scene* CreateScene(bool makeActive = false);
		static Scene* CreateScene(const UUID& sceneId, bool makeActive = false);
		static void DeleteScene(const UUID& sceneId);

		static void DebugScenes();
		
		static void LoadScene(const UUID& sceneId);
		static void UnloadScene(const UUID& sceneId);
	private:
		static std::vector<Scene> s_Scenes;
	};
}
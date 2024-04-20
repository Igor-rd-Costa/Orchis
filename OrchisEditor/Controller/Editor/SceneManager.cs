using OrchisEditor.Controller.Orchis;
using OrchisEditor.Controller.Utils;
using OrchisEditor.View.Editor;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace OrchisEditor.Controller.Editor
{
    internal class SceneManager
    {
        private readonly static List<Scene> s_Scenes = [];

        public static List<Scene> Scenes
        {
            get { return s_Scenes; }
        }

        public static bool LoadScenes(List<Tag> scenes)
        {
            foreach (Tag scene in scenes)
            {
                if (scene.Name != "Scene")
                    continue;

                string name = scene.GetAttribute("Name");
                if (name == "")
                {
                    //TODO handle error, could not find scene name
                    return false;
                }
                string id = scene.GetAttribute("Id");
                if (id == "")
                {
                    //TODO handle error, could not find scene id
                    return false;
                }
                bool isActive = bool.Parse(scene.GetAttribute("Active"));
                s_Scenes.Add(new(name, Guid.Parse(id), isActive));
                bool status = s_Scenes[^1].LoadEntities(scene);
                if (!status)
                    return false;
            }
            return true;
        }

        public static int AddScene(Guid sceneId, string name = "NewScene", bool isActive = false)
        {
            string sceneName = name;
            int sceneNameExt = 2;

            bool foundName = true;
            while (foundName)
            {
                foundName = false;
                foreach (Scene scene in s_Scenes)
                {
                    if (scene.Name == sceneName)
                    {
                        sceneName = name + sceneNameExt++;
                        foundName = true;
                        break;
                    }
                }
            }
            if (sceneId == Guid.Empty)
                s_Scenes.Add(new(sceneName, isActive));
            else
                s_Scenes.Add(new(sceneName, sceneId, isActive));
            Project.RegisterChange();
            EditorWindow.GetProjectOutliner()?.UpdateGUI();
            return s_Scenes.Count - 1;
        }

        public static void RemoveScene(Guid sceneId)
        {
            foreach (Scene scene in s_Scenes)
            {
                if (scene.Id == sceneId)
                {
                    OrchisInterface.OrchisSceneManagerDeleteScene(sceneId);
                    s_Scenes.Remove(scene);
                    Project.RegisterChange();
                    EditorWindow.GetProjectOutliner()?.UpdateGUI();
                    return;
                }
            }
        }

        public static void DebugScenes()
        {
            Console.WriteLine($"Editor:\nProject {Project.Name}: {s_Scenes.Count} scenes.");
            OrchisInterface.OrchisSceneManagerDebugScenes();
        }

        public static Scene? GetScene(Guid id)
        {
            foreach (Scene scene in s_Scenes)
            {
                if (scene.Id == id)
                    return scene;
            }
            return null;
        }
    }
}

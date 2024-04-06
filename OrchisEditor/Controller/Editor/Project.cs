using Microsoft.VisualBasic.FileIO;
using OrchisEditor.Controller.Orchis;
using OrchisEditor.Controller.Utils;
using OrchisEditor.View.Editor;
using System.IO;
using System.Text;
using System.Windows;


namespace OrchisEditor.Controller.Editor
{

    internal class Project
    {
        public enum ProjectTemplate
        {
            BLANK
        }

        public static void Create(ProjectTemplate template, string name, string projectPath)
        {
            if (s_HasUnsavedChanges)
                if (!QueryProjectSave())
                {
                    return;
                }

            switch(template)
            {
                case ProjectTemplate.BLANK: CreateBlankProject(name, projectPath); break;
            }

            ((App)Application.Current).SetLastProjectPath(projectPath);
        }

        public static bool Load(string projectPath)
        {
            if (s_HasUnsavedChanges)
                if (!QueryProjectSave())
                {
                    return false;
                }

            Tag? proj = Parser.ParseFileXML(projectPath);
            if (!proj.HasValue)
                return false;

            Tag project = proj.Value;
            if (project.Name != "Project")
            {
                //TODO handle error. wrong file sintax
                return false;
            }
            s_Name = project.GetAttribute("Name");
            if (s_Name == "")
            {
                //TODO handle error. wrong file sintax, could not find project name
                return false;
            }
            s_Version = project.GetAttribute("Version");
            if (s_Version == "")
            {
                //TODO handle error. wrong file sintax, could not find version
                return false;
            }

            Tag assets = project.Childs[0];
            if (assets.Name != "Assets")
            {
                //TODO handle error. wrong file sintax, could not find assets.
                return false;
            }
            
            Tag scenes = project.Childs[1];
            foreach (Tag scene in scenes.Childs)
            {
                if (scene.Name == "Scene")
                {
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
                    int sceneIndex = AddScene(name, int.Parse(id), isActive);
                    if (isActive) 
                        s_ActiveScene = s_Scenes[sceneIndex];    

                    foreach (Tag entity in scene.Childs)
                    {
                        string entityName = entity.GetAttribute("Name");
                        if (entityName == "")
                        {
                            //TODO handle error, could not find entity name
                            return false;
                        }
                        int entityIndex = s_Scenes[sceneIndex].AddEntity(entityName);
                        foreach (Tag componentTag in entity.Childs)
                        {
                            //TODO add components
                        }
                    }
                }
                else
                {
                    Console.WriteLine("Unrecognized element " + scene.Name + " ignored.");
                }
            }
            ((App)Application.Current).RegisterRecentProject(Name, projectPath);
            s_EditorCamera = OrchisInterface.OrchisCameraCreate();
            OrchisInterface.OrchisRendererSetActiveCamera(s_EditorCamera);
            s_Loaded = true;
            return true;
        }
        public static string Name
        {
            get { return s_Name; }
            set { s_Name = value; }
        }

        public static bool IsLoaded
        {
            get { return  s_Loaded; }
        }
        public static string Version { get { return s_Version; } }
        public static int GetSceneCount() { return s_Scenes.Count; }
        public static Scene? GetScene(string name) 
        { 
            foreach (Scene scene in s_Scenes)
            {
                if (scene.Name == name) return scene;
            }
            return null;
        }

        public static Scene GetScene(int index)
        {
            return s_Scenes[index];
        }

        public static int AddScene(string name = "NewScene", int id = 0, bool isActive = false) 
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

            s_Scenes.Add(new(sceneName, id));
            if (((EditorWindow)Application.Current.MainWindow).ProjectOutliner != null)
            {
                ((EditorWindow)Application.Current.MainWindow).ProjectOutliner.UpdateGUI();
            }
            s_HasUnsavedChanges = true;
            return s_Scenes.Count - 1;
        }
        public static void RemoveScene(int index) 
        { 
            s_Scenes.RemoveAt(index);
            s_HasUnsavedChanges = true;
        }

        public static void RemoveScene(string name)
        {
            for (int i = 0; i < s_Scenes.Count; i++)
            {
                if (s_Scenes[i].Name == name)
                {
                    s_Scenes.RemoveAt(i);
                    break;
                }
            }
            ((EditorWindow)Application.Current.MainWindow).ProjectOutliner.UpdateGUI();
            
        }


        private static void CreateBlankProject(string name, string projectPath)
        {
            FileSystem.CreateDirectory(projectPath + name);
            projectPath = projectPath + name + "\\" + name + s_ProjExtension;
            FileStream fileStream = File.Create(projectPath);

            byte[] buffer = new UTF8Encoding(true).GetBytes(
                "<Project Name=\"" + name + "\" " + "Version=\"" + s_ProjVersion + "\">\n" +
                "<Assets>\n" +
                "</Assets>\n" +
                "<Scenes>\n" +
                "\t<Scene Id=\"0\" Name=\"NewScene\" Active=\"true\">\n" +
                "\t</Scene>\n" +
                "</Scenes>\n" +
                "</Project>");

            fileStream.Write(buffer, 0, buffer.Length);
            fileStream.Flush();
            fileStream.Dispose();

            Load(projectPath);
        }

        //TODO implement saving
        private static bool QueryProjectSave()
        {

            return true;
        }

        static public Scene? ActiveScene
        {
            get { return s_ActiveScene; }
        }

        private static IntPtr s_EditorCamera = IntPtr.Zero;
        public static bool s_HasUnsavedChanges = false;
        private static string s_Name = "";
        private static string s_Version = "0.0.0";
        private static List<Scene> s_Scenes= new();
        private static Scene? s_ActiveScene;
        private static string s_ProjectsDirPath = "Projects/";
        private static bool s_Loaded = false;
        private const string s_ProjExtension = ".orcproj";
        private const string s_ProjVersion = "0.0.1";

    }

}

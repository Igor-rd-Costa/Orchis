using Microsoft.VisualBasic.FileIO;
using OrchisEditor.Controller.Orchis;
using OrchisEditor.View.Editor;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Windows;
using System.Xml.Linq;

namespace OrchisEditor.Controller.Editor
{

    internal class Project
    {
        public enum ProjectTemplate
        {
            BLANK
        }

        public static void Create(ProjectTemplate template, string name, string filePath = "")
        {
            if (s_HasUnsavedChanges)
                if (!QueryProjectSave())
                {
                    return;
                }

            switch(template)
            {
                case ProjectTemplate.BLANK: CreateBlankProject(name); break;
            }
        }

        public static bool Load(string projectPath)
        {
            Tag? project = Parser.ParseProject(projectPath);

            if (s_HasUnsavedChanges)
                if (!QueryProjectSave())
                {
                    return false;
                }
           

            if (!project.HasValue)
                return false;

            if (project.Value.Name != "Project")
            {
                //TODO handle error. wrong file sintax
                return false;
            }
            s_Name = GetAttribute("Name", project.Value);
            if (s_Name == "")
            {
                //TODO handle error. wrong file sintax, could not find project name
                return false;
            }
            s_Version = GetAttribute("Version", project.Value);
            if (s_Version == "")
            {
                //TODO handle error. wrong file sintax, could not find version
                return false;
            }
            foreach (Tag sceneTag in project.Value.Childs)
            {
                if (sceneTag.Name == "Scene")
                {
                    string name = GetAttribute("Name", sceneTag);
                    if (name == "")
                    {
                        //TODO handle error, could not find scene name
                        return false;
                    }
                    string id = GetAttribute("Id", sceneTag);
                    if (id == "")
                    {
                        //TODO handle error, could not find scene id
                        return false;
                    }
                    bool isActive = bool.Parse(GetAttribute("Active", sceneTag));
                    int sceneIndex = AddScene(name, int.Parse(id), isActive);
                    if (isActive) 
                        s_ActiveScene = s_Scenes[sceneIndex];    

                    foreach (Tag entityTag in sceneTag.Childs)
                    {
                        string entityName = GetAttribute("Name", entityTag);
                        if (entityName == "")
                        {
                            //TODO handle error, could not find entity name
                            return false;
                        }
                        int entityIndex = s_Scenes[sceneIndex].AddEntity(entityName);
                        foreach (Tag componentTag in entityTag.Childs)
                        {
                            //TODO add components
                        }
                    }
                }
                else
                {
                    Console.Write("Unrecognized element " + sceneTag.Name + " ignored.");
                }
            }

            s_EditorCamera = OrchisInterface.OrchisCameraCreate();
            OrchisInterface.OrchisRendererSetActiveCamera(s_EditorCamera);
            s_Loaded = true;
            return true;
        }

        static string GetAttribute(string name, Tag tag)
        {
            foreach (TagAttribute attribute in tag.Attributes) 
            {
                if (attribute.Name == name)
                    return attribute.Value;
            }
            return "";
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


        private static void CreateBlankProject(string name)
        {

            if (!FileSystem.DirectoryExists(s_ProjectsDirPath))
                FileSystem.CreateDirectory(s_ProjectsDirPath);

            FileStream fileStream = File.Create(s_ProjectsDirPath + name + s_ProjExtension);

            byte[] buffer = new UTF8Encoding(true).GetBytes(
                "<Project Name=\"" + name + "\" " + "Version=\"" + s_ProjVersion + "\">" +
                "\n\t<Scene Id=\"0\" Name=\"NewScene\" Active=\"true\">\n" +
                  "\t</Scene>\n" +
                "</Project>");

            fileStream.Write(buffer, 0, buffer.Length);
            fileStream.Flush();
            fileStream.Dispose();

            Load(s_ProjectsDirPath + name + s_ProjExtension);
        }
        private static bool QueryProjectSave()
        {
            //TODO implement saving

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

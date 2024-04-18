using Microsoft.VisualBasic.FileIO;
using OrchisEditor.Controller.Editor.Components;
using OrchisEditor.Controller.Orchis;
using OrchisEditor.Controller.Utils;
using OrchisEditor.View.Editor;
using OrchisEditor.View.UserControls;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Windows;
using System.Xml.Linq;


namespace OrchisEditor.Controller.Editor
{

    internal class Project
    {
        private static IntPtr s_EditorCamera = IntPtr.Zero;
        public static bool s_HasUnsavedChanges = false;
        private static string s_Name = "";
        private static bool s_Loaded = false;
        private const string s_ProjExtension = ".orcproj";
        private const string s_ProjVersion = "0.0.1";
        private static string s_ProjPath = "";
        private static List<Action<bool>> m_OnChangeCallbacks = [];

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
                    return false;

            Tag? proj = Parser.ParseFileXML(projectPath);
            if (!proj.HasValue)
                return false;

            Tag project = proj.Value;
            if (project.Name != "OrchisProject")
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
            string version = project.GetAttribute("Version");
            if (version == "")
            {
                //TODO handle error. wrong file sintax, could not find version
                return false;
            }
            if (version != s_ProjVersion)
            {
                //TODO wrong version. handle update.
                return false;
            }

            Tag assets = project.Childs[0];
            if (assets.Name != "Assets")
            {
                //TODO handle error. wrong file sintax, could not find assets.
                return false;
            }
            
            Tag scenes = project.Childs[1];
            if (scenes.Name != "Scenes")
            {
                //TODO handle error. wrong file sintax, could not find assets.
                return false;
            }
            SceneManager.LoadScenes(scenes.Childs);
            ((App)Application.Current).RegisterRecentProject(Name, projectPath);
            s_ProjPath = projectPath.Substring(0, projectPath.LastIndexOf('\\') + 1);
            s_EditorCamera = Engine.Camera.Create();
            OrchisInterface.OrchisRendererSetActiveCamera(s_EditorCamera);
            Console.WriteLine($"Project \"{Name}\" Loaded!");
            s_Loaded = true;
            return true;
        }

        public static bool OnAppClose()
        {
            if (s_HasUnsavedChanges)
                return QueryProjectSave();
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
        public static string Version { get { return s_ProjVersion; } }

        private static void CreateBlankProject(string name, string projectPath)
        {
            FileSystem.CreateDirectory(projectPath + name);
            FileSystem.CreateDirectory(projectPath + name + "\\Assets");
            File.Create(projectPath + name + "\\Assets\\info.oai");
            projectPath = projectPath + name + "\\" + name + s_ProjExtension;
            FileStream fileStream = File.Create(projectPath);
            Tag newProject = new()
            {
                Name = "OrchisProject",
                Attributes = [new TagAttribute("Name", name), new TagAttribute("Version", s_ProjVersion)],
                Childs = [new Tag("Assets"),
                    new Tag("Scenes", [], [
                        new Tag("Scene", [
                            new TagAttribute("Id", Guid.NewGuid().ToString().ToUpper()),
                            new TagAttribute("Name", "NewScene"),
                            new TagAttribute("Active", "true")], [])
                        ])
                ]
            };
            Console.WriteLine("Editor: Creating project with scene id " + newProject.Childs[1].Childs[0].GetAttribute("Id"));

            var buffer = new UTF8Encoding(true).GetBytes(Parser.ToXML(newProject));
            fileStream.Write(buffer, 0, buffer.Length);
            fileStream.Flush();
            fileStream.Dispose();

            Load(projectPath);
        }

        //TODO implement saving
        private static bool QueryProjectSave()
        {
            OrchisSaveDialog saveDialog = new OrchisSaveDialog();
            saveDialog.ShowDialog();
            SaveDialogResult result = saveDialog.Result;
            if (result == SaveDialogResult.CANCEL)
                return false;

            if (result == SaveDialogResult.SAVE)
            {
                Save();
            }
            return true;
        }

        public static void Save()
        {
            if (!s_HasUnsavedChanges)
                return;

            string projFilePath = s_ProjPath + Name + s_ProjExtension;
            if (!File.Exists(projFilePath))
            {
                OrchisDialog dialog = new();
                dialog.SetDialogType(OrchisDialogType.OK);
                dialog.ShowMessage($"Unable to find project file at {projFilePath}.", "Error");
                // TODO add a way to locate file manually
                return;
            }
            Tag project = new(
                "OrchisProject",
                [
                    new TagAttribute("Name", Name), 
                    new TagAttribute("Version", s_ProjVersion)
                ], 
                [
                    new ("Assets") //TODO add assets
                ]);
            Tag scenes = new("Scenes");
            foreach (Scene scene in SceneManager.Scenes)
            {
                scenes.Childs.Add(new(
                    "Scene",
                    [
                        new("Id", scene.Id.ToString().ToUpper()),
                        new("Name", scene.Name),
                        new("Active", scene.Active.ToString())
                    ], ParseSceneEntities(scene)));
            }
            project.Childs.Add(scenes);
            FileStream fs = File.Open(projFilePath, FileMode.Open);
            fs.SetLength(0);
            fs.Write(new UTF8Encoding().GetBytes(Parser.ToXML(project)));
            fs.Dispose();
            s_HasUnsavedChanges = false;
            foreach (Action<bool> changeCallback in m_OnChangeCallbacks)
                changeCallback(s_HasUnsavedChanges);
        }

        private static List<Tag> ParseSceneEntities(Scene scene)
        {
            List<Tag> entities = [];
            foreach (Entity entity in scene.Entities)
            {
                entities.Add(new(
                    "Entity",
                        [
                            new("Id", entity.Id.ToString().ToUpper()),
                            new("Name", entity.Name)
                        ], ParseEntityComponents(entity)));
            }
            return entities;
        }

        private static List<Tag> ParseEntityComponents(Entity entity)
        {
            List<Tag> components = [];
            foreach (Component component in entity.Components)
            {
                Tag c = new("Component");
                c.SetAttribute("Type", component.Type.ToString());
                c.SetAttribute("Id", component.Id.ToString().ToUpper());
                c.AddAttributes(GetComponentAttributes(component));
                components.Add(c);
            }
            return components;
        }

        private static List<TagAttribute> GetComponentAttributes(Component component)
        {
            List<TagAttribute> attrbs = [];
            switch (component.Type)
            {
                case ComponentType.TRANSFORM:
                {
                    TransformComponent c = OrchisInterface.OrchisComponentManagerGetTransformComponent(component.Id);
                    attrbs.Add(new("Position", $"{c.Position.X},{c.Position.Y},{c.Position.Z}"));
                    attrbs.Add(new("Scale", $"{c.Scale.X},{c.Scale.Y},{c.Scale.Z}"));
                    attrbs.Add(new("Rotation", $"{c.Rotation.X},{c.Rotation.Y},{c.Rotation.Z}"));
                } break;
                default: 
                {
                    Debug.Assert(false, "Component type not implemented.");
                } break;
            }
            return attrbs;
        }

        public static void RegisterChange()
        {
            if (!s_HasUnsavedChanges)
            {
                s_HasUnsavedChanges = true;
                foreach (Action<bool> changeCallback in m_OnChangeCallbacks)
                    changeCallback(s_HasUnsavedChanges);
            }
        }

        public static void OnProjectChange(Action<bool> onProjectChange)
        {
            m_OnChangeCallbacks.Add(onProjectChange);
        }
    }

}

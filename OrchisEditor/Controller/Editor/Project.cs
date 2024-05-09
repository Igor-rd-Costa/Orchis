using Microsoft.VisualBasic.FileIO;
using OrchisEditor.Controller.Editor.Components;
using OrchisEditor.Controller.Orchis;
using OrchisEditor.Controller.Utils;
using OrchisEditor.View.Editor;
using OrchisEditor.View.Editor.UserControls.ToolsPanelComponents;
using OrchisEditor.View.UserControls;
using System.Diagnostics;
using System.IO;
using System.Numerics;
using System.Text;
using System.Windows;


namespace OrchisEditor.Controller.Editor
{
    internal struct ProjectSettings
    {
        public string Name;
        public EditorCameraData CameraData;
        public Guid SceneManagerCurrentScene;
    }
    internal class Project
    {
        private static ProjectSettings m_ProjectSettings = new();
        public static bool s_HasUnsavedChanges = false;
        private static bool s_Loaded = false;
        private const string s_ProjExtension = ".orcproj";
        private const string s_ProjVersion = "0.0.1";
        private static string s_ProjPath = "";
        private static List<Action<bool>> m_OnChangeCallbacks = [];

        public enum ProjectTemplate
        {
            BLANK
        }
        public static ProjectSettings Settings
        {
            get { return m_ProjectSettings; }
        }
        public static string Name
        {
            get { return m_ProjectSettings.Name; }
            set { m_ProjectSettings.Name = value; }
        }

        public static bool IsLoaded
        {
            get { return s_Loaded; }
        }
        public static string Version { get { return s_ProjVersion; } }

        public static string ProjectPath { get { return s_ProjPath; } }

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

            FileStream stream = File.Open(projectPath, FileMode.Open);
            Tag? proj = Parser.ParseXML(new StreamReader(stream).ReadToEnd());
            stream.Dispose();
            if (!proj.HasValue)
                return false;

            Tag project = proj.Value;
            if (project.Name != "OrchisProject")
            {
                //TODO handle error. wrong file sintax
                return false;
            }
            m_ProjectSettings.Name = project.GetAttribute("Name");
            if (Settings.Name == "")
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
                throw new NotImplementedException("Project file updates not implemented");
            }

            foreach (Tag tag in project.Childs)
            {
                if (tag.Name == "EditorCamera")
                {
                    string[] posString = tag.GetAttribute("Position").Split(',');
                    string[] lookAtString = tag.GetAttribute("LookAt").Split(',');
                    Vector3 pos = new(float.Parse(posString[0]), float.Parse(posString[1]), float.Parse(posString[2]));
                    Vector3 lookAt = new(float.Parse(lookAtString[0]), float.Parse(lookAtString[1]), float.Parse(lookAtString[2]));
                    float speed = float.Parse(tag.GetAttribute("Speed"));
                    float fielOfView = float.Parse(tag.GetAttribute("FieldOfView"));
                    float yaw = float.Parse(tag.GetAttribute("Yaw"));
                    float pitch = float.Parse(tag.GetAttribute("Pitch"));
                    m_ProjectSettings.CameraData = new()
                    {
                        Position = pos,
                        LookAt = lookAt,
                        Speed = speed,
                        FieldOfView = fielOfView,
                        Yaw = yaw,
                        Pitch = pitch
                    };
                }
                if (tag.Name == "SceneManager")
                {
                    Guid sceneId = Guid.Parse(tag.GetAttribute("CurrentScene"));
                    m_ProjectSettings.SceneManagerCurrentScene = sceneId;
                }
                if (tag.Name == "AssetManager")
                {
                    string currentPath = tag.GetAttribute("CurrentPath");
                    AssetManagerView.CurrentPath = currentPath;
                }
            }

            ((App)Application.Current).RegisterRecentProject(Name, projectPath);
            s_ProjPath = projectPath.Substring(0, projectPath.LastIndexOf('\\') + 1);
            s_Loaded = true;
            return true;
        }

        public static bool OnAppClose()
        {
            if (s_HasUnsavedChanges)
                return QueryProjectSave();
            return true;
        }
        private static void CreateBlankProject(string name, string projectPath)
        {
            string assetsPath = projectPath + name + "\\Assets";
            FileSystem.CreateDirectory(projectPath + name);
            FileSystem.CreateDirectory(assetsPath);
            projectPath = $"{projectPath}{name}\\{name}{s_ProjExtension}";
            FileStream fileStream = File.Create(projectPath);
            Tag newProject = new()
            {
                Name = "OrchisProject",
                Attributes = [new("Name", name), new("Version", s_ProjVersion)],
                Childs = [
                    new() {
                        Name = "EditorCamera",
                        Attributes = [
                            new("Position", "0,0,0"), new("LookAt", "0,1,0"), new ("Speed", "5"), 
                            new("FieldOfView", "45"), new("Yaw", "0"), new ("Pitch", "0")
                        ]
                    },
                    new() {
                        Name = "SceneManager",
                        Attributes = [ new("CurrentScene", Guid.Empty.ToString()) ]
                    },
                    new() {
                        Name = "AssetManager",
                        Attributes = [ new("CurrentPath", assetsPath) ]
                    }
                ]
            };
            var buffer = new UTF8Encoding(true).GetBytes(Parser.ToXML(newProject));
            fileStream.Write(buffer, 0, buffer.Length);
            fileStream.Flush();
            fileStream.Dispose();

            Load(projectPath);
        }

        public static bool QueryProjectSave()
        {
            if (!s_HasUnsavedChanges)
                return true;

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
        public static void UpdateAssetManagerCurrentPath()
        {
            FileStream stream = OpenProjectFile();
            Tag? project = Parser.ParseXML(new StreamReader(stream).ReadToEnd());
            if (!project.HasValue)
            {
                stream.Dispose();
                return;
            }
            foreach (Tag child in project.Value.Childs)
            {
                if (child.Name == "AssetManager")
                {
                    child.SetAttribute("CurrentPath", AssetManagerView.CurrentPath);
                    stream.SetLength(0);
                    stream.Write(new UTF8Encoding().GetBytes(Parser.ToXML(project.Value)));
                    stream.Flush();
                    stream.Dispose();
                    return;
                }
            }
            stream.Dispose();
        }

        private static FileStream OpenProjectFile()
        {
            return File.Open(s_ProjPath + Name + s_ProjExtension, FileMode.Open);
        }

        public static void SaveProjectSettings()
        {
            FileStream stream = OpenProjectFile();
            Tag? project = Parser.ParseXML(new StreamReader(stream).ReadToEnd());
            if (!project.HasValue)
            {
                stream.Dispose();
                return;
            }
            project.Value.SetAttribute("Name", Settings.Name);
            project.Value.SetAttribute("Version", s_ProjVersion);
            foreach (Tag child in project.Value.Childs)
            {
                switch (child.Name)
                {
                    case "EditorCamera":
                    {
                        EditorCameraData data = Engine.Editor.GetCameraData();
                        child.SetAttribute("Position", $"{data.Position.X},{data.Position.Y},{data.Position.Z}");
                        child.SetAttribute("LookAt", $"{data.LookAt.X},{data.LookAt.Y},{data.LookAt.Z}");
                        child.SetAttribute("Speed", data.Speed.ToString());
                        child.SetAttribute("FieldOfView", data.FieldOfView.ToString());
                        child.SetAttribute("Yaw", data.Yaw.ToString());
                        child.SetAttribute("Pitch", data.Pitch.ToString());
                    }
                    break;
                    case "SceneManager":
                    {
                        child.SetAttribute("CurrentScene", SceneManager.Scene == null ? Guid.Empty.ToString() : SceneManager.Scene.Id.ToString().ToUpper());
                    }
                    break;
                    case "AssetManager":
                    {
                        child.SetAttribute("CurrentPath", AssetManagerView.CurrentPath);
                    }
                    break;
                }
            }
            stream.SetLength(0);
            stream.Write(new UTF8Encoding().GetBytes(Parser.ToXML(project.Value)));
            stream.Flush();
            stream.Dispose();
        }

        public static void Save()
        {
            SaveProjectSettings();
            if (!s_HasUnsavedChanges)
                return;

            SceneManager.Save();
            AssetManager.Save();
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
                case ComponentType.MESH:
                {
                    MeshComponent c = OrchisInterface.OrchisComponentManagerGetMeshComponent(component.Id);
                    attrbs.Add(new("MeshType", c.MeshType.ToString()));
                    attrbs.Add(new("MeshId", c.MeshId.ToString().ToUpper()));
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

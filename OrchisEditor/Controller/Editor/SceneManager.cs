using OrchisEditor.Controller.Editor.Components;
using OrchisEditor.Controller.Orchis;
using OrchisEditor.Controller.Utils;
using OrchisEditor.View.Editor;
using OrchisEditor.View.UserControls;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace OrchisEditor.Controller.Editor
{
    internal class SceneManager
    {
        private static bool s_HasUnsavedChanges = false;
        private static Scene? s_Scene = null;


        public static Scene? Scene { get { return s_Scene; } }
        public static void Init()
        {
            if (!Directory.Exists($"{AssetManager.Path}\\Scenes"))
                Directory.CreateDirectory($"{AssetManager.Path}\\Scenes");
            if (!File.Exists($"{AssetManager.Path}\\Scenes\\scenes.osi"))
            {
                FileStream fs = File.Create($"{AssetManager.Path}\\Scenes\\scenes.osi");
                Tag tag = new Tag("SceneInfo");
                fs.Write(new UTF8Encoding().GetBytes(Parser.ToXML(tag)));
                fs.Flush();
                fs.Dispose();
            }
            else
            {
                LoadScene(Project.Settings.SceneManagerCurrentScene);
            }
        }
        public static void CreateScene()
        {
            if (Project.QueryProjectSave())
            {
                Guid sceneId = Guid.NewGuid();
                string scenePath = $"{AssetManager.Path}\\Scenes\\";
                string scenePrefix = "NewScene";
                string sceneName = scenePrefix;
                int count = 2;
                while (File.Exists(scenePath+sceneName+".osn")) 
                {
                    sceneName = $"{scenePrefix}({count})";
                    count++;
                }
                scenePath = scenePath + sceneName + ".osn";
                FileStream fs = File.Create(scenePath);
                Tag scene = new("Scene",
                    [
                        new("Name", sceneName),
                        new("Id", sceneId.ToString().ToUpper()),
                    ], 
                    []);
                fs.Write(new UTF8Encoding().GetBytes(Parser.ToXML(scene)));
                fs.Flush(true);
                fs.Dispose();
                RegisterSceneEntry(sceneId, scenePath);
                LoadScene(scenePath);
                RegisterChange();
            }
        }

        public static void LoadScene(string scenePath)
        {
            if (!File.Exists(scenePath))
                return;

            if (!Project.QueryProjectSave())
                return;

            FileStream fs = File.Open(scenePath, FileMode.Open);
            Tag? scene = Parser.ParseXML(new StreamReader(fs).ReadToEnd());
            fs.Dispose();
            if (!scene.HasValue)
            {
                Debug.Assert(false, $"Invalid scene {scenePath}.");
                return;
            }

            Guid sceneId = Guid.Parse(scene.Value.GetAttribute("Id"));
            if (s_Scene != null && s_Scene.Id == sceneId)
                return;
            
            string sceneName = scene.Value.GetAttribute("Name");
            s_Scene = new Scene(sceneName, sceneId);
            s_Scene.LoadEntities(scene.Value);
            EditorWindow.GetProjectOutliner()?.UpdateGUI();
        }

        public static void LoadScene(Guid sceneId)
        {
            if (sceneId == Guid.Empty)
                return;

            FileStream fs = File.Open($"{AssetManager.Path}\\Scenes\\scenes.osi", FileMode.Open);
            string content = new StreamReader(fs).ReadToEnd();
            Tag? tag = Parser.ParseXML(content);
            fs.Dispose();
            if (tag.HasValue)
            {
                foreach (Tag child in tag.Value.Childs)
                {
                    if (Guid.Parse(child.GetAttribute("Id")) == sceneId)
                    {
                        LoadScene(child.GetAttribute("Path"));
                        break;
                    }
                }
            }
        }

        private static void RegisterSceneEntry(Guid sceneId, string scenePath)
        {
            if (!File.Exists($"{AssetManager.Path}\\Scenes\\scenes.osi"))
            {
                return;
            }
            FileStream fs = File.Open($"{AssetManager.Path}\\Scenes\\scenes.osi", FileMode.Open);
            Tag? tag = Parser.ParseXML(new StreamReader(fs).ReadToEnd());
            if (tag.HasValue)
            {
                tag.Value.Childs.Add(new("Scene",
                [
                    new("Id", sceneId.ToString().ToUpper()),
                    new("Path", scenePath)
                ], []));
                fs.SetLength(0);
                fs.Write(new UTF8Encoding().GetBytes(Parser.ToXML(tag.Value)));
                fs.Flush();
                fs.Dispose();
            }
        }

        public static void RegisterChange()
        {
            s_HasUnsavedChanges = true;
            Project.RegisterChange();
        }

        public static void Save()
        {
            if (!s_HasUnsavedChanges || Scene == null)
                return;

            if (!File.Exists($"{AssetManager.Path}\\Scenes\\{Scene.Name}.osn"))
            {
                Debug.Assert(false); 
                return;
            }

            FileStream fs = File.Open($"{AssetManager.Path}\\Scenes\\{Scene.Name}.osn", FileMode.Open);
            Tag scene = new Tag("Scene",
                [
                new ("Name", Scene.Name),
                new ("Id", Scene.Id.ToString().ToUpper()),
                ], SaveEntities());
            fs.SetLength(0);
            fs.Write(new UTF8Encoding().GetBytes(Parser.ToXML(scene)));
            fs.Flush(true);
            fs.Dispose();
            s_HasUnsavedChanges = false;
        }

        private static List<Tag> SaveEntities()
        {
            List<Tag> entities = [];
            if (Scene == null)
                return entities;

            foreach (Entity entity in Scene.Entities)
            {
                Tag e = new("Entity",
                    [
                    new ("Name", entity.Name),
                    new ("Id", entity.Id.ToString().ToUpper()),
                    ], SaveComponents(entity));
                entities.Add(e);
            }
            
            return entities;
        }

        private static List<Tag> SaveComponents(Entity entity)
        {
            List<Tag> components = [];

            foreach (Component component in entity.Components)
            {
                Tag c = new("Component",
                    [
                    new ("Id", component.Id.ToString().ToUpper()),
                    new ("Type", component.Type.ToString()),
                    ], []);
                c.AddAttributes(GetComponentAttributes(component));
                components.Add(c);
            }

            return components;
        }

        private static List<TagAttribute> GetComponentAttributes(Component component)
        {
            List<TagAttribute> attributes = [];
            switch (component.Type)
            {
                case ComponentType.TRANSFORM:
                {
                    TransformComponent tc = OrchisInterface.OrchisComponentManagerGetTransformComponent(component.Id);
                    if (tc.Id == Guid.Empty) break;
                    attributes.Add(new("Position", $"{tc.Position.X},{tc.Position.Y},{tc.Position.Z}"));
                    attributes.Add(new("Rotation", $"{tc.Rotation.X},{tc.Rotation.Y},{tc.Rotation.Z}"));
                    attributes.Add(new("Scale", $"{tc.Scale.X},{tc.Scale.Y},{tc.Scale.Z}"));
                } break;
                case ComponentType.MESH:
                {
                    MeshComponent mc = OrchisInterface.OrchisComponentManagerGetMeshComponent(component.Id);
                    if (mc.Id == Guid.Empty) break;
                    attributes.Add(new("MeshId", mc.MeshId.ToString().ToUpper()));
                    attributes.Add(new("MeshType", mc.MeshType.ToString()));
                } break;
                case ComponentType.TEXTURE:
                {
                    TextureComponent tc = OrchisInterface.OrchisComponentManagerGetTextureComponent(component.Id);
                    if (tc.Id == Guid.Empty) break;
                    attributes.Add(new("TextureId", tc.TextureId.ToString().ToUpper()));
                } break;
            }
            return attributes;
        }

        public static bool RenameScene(string newName)
        {
            if(s_Scene != null)
            {
                if (File.Exists($"{AssetManager.Path}\\Scenes\\{newName}.osn"))
                {
                    OrchisDialog dialog = new(OrchisDialogType.OK);
                    dialog.ShowMessage("A scene with this name already exists.");
                    return false;
                }
                FileStream stream = File.Open($"{AssetManager.Path}\\Scenes\\{s_Scene.Name}.osn", FileMode.Open);
                Tag? scene = Parser.ParseXML(new StreamReader(stream).ReadToEnd());
                if (!scene.HasValue || scene.Value.Name != "Scene")
                    return false;
                scene.Value.SetAttribute("Name", newName);
                stream.SetLength(0);
                stream.Write(new UTF8Encoding().GetBytes(Parser.ToXML(scene.Value)));
                stream.Flush();
                stream.Dispose();
                File.Move($"{AssetManager.Path}\\Scenes\\{s_Scene.Name}.osn", $"{AssetManager.Path}\\Scenes\\{newName}.osn");
                stream = File.Open($"{AssetManager.Path}\\Scenes\\scenes.osi", FileMode.Open);
                Tag? sceneInfo = Parser.ParseXML(new StreamReader(stream).ReadToEnd());
                if (sceneInfo.HasValue)
                {
                    foreach (Tag info in sceneInfo.Value.Childs)
                    {
                        if (Guid.Parse(info.GetAttribute("Id")) == s_Scene.Id)
                        {
                            info.SetAttribute("Path", $"{AssetManager.Path}\\Scenes\\{newName}.osn");
                            stream.SetLength(0);
                            stream.Write(new UTF8Encoding().GetBytes(Parser.ToXML(sceneInfo.Value)));
                            stream.Flush();
                            break;
                        }
                    }
                }
                stream.Dispose();
                s_Scene.Name = newName;
                return true;
            }
            return false;
        }

        public static bool RenameEntity(Guid entityId, string newName)
        {
            if (s_Scene == null)
                return false;

            foreach (Entity entity in s_Scene.Entities)
            {
                if (entity.Id == entityId)
                {
                    entity.Name = newName;
                    RegisterChange();
                    return true;
                }
            }
            return false;
        }
    }
}

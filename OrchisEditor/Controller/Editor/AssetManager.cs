using OrchisEditor.Controller.Editor.Components;
using OrchisEditor.Controller.Orchis;
using OrchisEditor.Controller.Utils;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrchisEditor.Controller.Editor
{
    public enum AssetChangeType
    {
        CREATED, DELETED, CHANGED, RENAMED, 
    }
    public enum AssetType
    {
        FOLDER, MESH, AUDIO, GENERIC
    }
    public struct AssetInfo
    {
        public string Name;
        public string Path;
    }
    public class AssetChangeEventArgs
    {
        private string m_AssetName;
        private string m_AssetPath;
        private AssetChangeType m_ChangeType;

        public AssetChangeEventArgs(string assetName, string assetPath, AssetChangeType changeType)
        {
            m_AssetName = assetName;
            m_AssetPath = assetPath;
            m_ChangeType = changeType;
        }

        public string Name { get { return m_AssetName; } }
        public string Path { get { return m_AssetPath; } }
        public AssetChangeType Change {  get { return m_ChangeType; } }
    }

    internal class AssetManager
    {
        private static string s_AssetsFolderPath = "";
        private static FileSystemWatcher? s_Watcher = null;
        private static List<Action<AssetChangeEventArgs>> s_AssetChangeCallbacks = [];
        private static FileStream? s_InfoStream = null;
        private AssetManager() { }

        public static void Init()
        {
            if (Project.IsLoaded)
            { 
                s_AssetChangeCallbacks.Clear();
                s_AssetsFolderPath = $"{Project.ProjectPath}Assets\\";
                s_Watcher = new FileSystemWatcher(s_AssetsFolderPath);
                s_Watcher.Created += OnWatcherCreated;
                s_Watcher.Deleted += OnWatcherDeleted;
                s_Watcher.Renamed += OnWatcherRenamed;
                s_Watcher.Changed += OnWatcherChanged;
                s_Watcher.EnableRaisingEvents = true;
                if (!File.Exists($"{Path}info.oai"))
                {
                    s_InfoStream = File.Create($"{Path}info.oai");
                    Tag tag = new("AssetInfo");
                    s_InfoStream.Write(new UTF8Encoding().GetBytes(Parser.ToXML(tag)));
                    s_InfoStream.Flush();
                }
                else
                {
                    //  s_InfoStream = File.Open($"{Path}info.oai", FileMode.Open);
                    OrchisInterface.OrchisFileSystemMount(Path.Substring(0, Path.Length - 1), "Assets");
                }
            }
        }
        public static void Shudown()
        {
            s_InfoStream?.Dispose();
            s_Watcher?.Dispose();
        }

        public static string Path { get { return s_AssetsFolderPath; } }

        public static Guid RegisterAsset(string assetPath)
        {
            if (!File.Exists($"{Path}info.oai"))
            {
                File.Create($"{Path}info.oai");
            }
            FileStream stream = File.Open($"{Path}info.oai", FileMode.Open);
            Tag? info = Parser.ParseXML(new StreamReader(stream).ReadToEnd());
            if (info.HasValue == false)
            {
                stream.Dispose();
                return Guid.Empty;
            }
            for (int i = 0; i < info.Value.Childs.Count; i++)
            {
                if (info.Value.Childs[i].GetAttribute("Path") == assetPath)
                {
                    stream.Dispose();
                    return Guid.Parse(info.Value.Childs[i].GetAttribute("Id"));
                }
            }
            Guid assetId = Guid.NewGuid();
            string assetTagName = GetAssetTagName(assetPath);
            info.Value.Childs.Add(new()
            {
                Name = assetTagName,
                Attributes = 
                { 
                    new("Id", assetId.ToString().ToUpper()),
                    new("Path", assetPath),
                }
            });
            info.Value.Childs[^1].AddAttributes(GetAssetAttributes(assetTagName));
            stream.SetLength(0);
            stream.Write(new UTF8Encoding().GetBytes(Parser.ToXML(info.Value)));
            stream.Flush();
            stream.Dispose();
            return assetId;
        }

        public static AssetInfo? GetAssetInfo(Guid assetId)
        {
            Tag? infoFile = OpenInfoFile();
            if (!infoFile.HasValue)
                return null;
            AssetInfo info = new();
            foreach (Tag asset in infoFile.Value.Childs)
            {
                if (Guid.Parse(asset.GetAttribute("Id")) == assetId)
                {
                    string path = asset.GetAttribute("Path");
                    info.Path = path;
                    int barIndex = path.LastIndexOf('\\');
                    info.Name = path.Substring(barIndex + 1, path.Length - barIndex - 1);
                    return info;
                }
            }
            return null;
        }

        public static string GetAssetTagName(string path)
        {
            int dot = path.LastIndexOf('.');
            string ext = path.Substring(dot, path.Length - dot).ToLower();
            if (ext == ".obj" || ext == ".fbx")
                return "Mesh";
            if (ext == ".png")
                return "Texture";
            return "Asset";
        }

        private static List<TagAttribute> GetAssetAttributes(string tagName)
        {
            List<TagAttribute> attributes = [];
            if (tagName == "Mesh")
                attributes.Add(new("Type", MeshType.STATIC.ToString()));

            return attributes;
        }

        public static void Save()
        {

        }

        public static void OnChange(Action<AssetChangeEventArgs> onChange)
        {
            s_AssetChangeCallbacks.Add(onChange);
        }

        private static void OnWatcherCreated(object sender, FileSystemEventArgs e)
        {
            TriggerCallbacks(e.Name ?? "", e.FullPath, AssetChangeType.CREATED);
        }

        private static void OnWatcherDeleted(object sender, FileSystemEventArgs e)
        {
            TriggerCallbacks(e.Name ?? "", e.FullPath, AssetChangeType.DELETED);
        }

        private static void OnWatcherChanged(object sender, FileSystemEventArgs e)
        {
            TriggerCallbacks(e.Name ?? "", e.FullPath, AssetChangeType.CHANGED);
        }

        private static void OnWatcherRenamed(object sender, FileSystemEventArgs e)
        {
            TriggerCallbacks(e.Name ?? "", e.FullPath, AssetChangeType.RENAMED);
        }

        private static void TriggerCallbacks(string assetName, string assetPath, AssetChangeType type)
        {
            AssetChangeEventArgs args = new AssetChangeEventArgs(assetName, assetPath, type);
            foreach (Action<AssetChangeEventArgs> onChange in s_AssetChangeCallbacks)
                onChange(args);
        }

        private static Tag? OpenInfoFile()
        {
            if (!File.Exists($"{Path}info.oai"))
                return null;
            s_InfoStream = File.Open($"{Path}info.oai", FileMode.Open);
            s_InfoStream.Position = 0;
            Tag? tag = Parser.ParseXML(new StreamReader(s_InfoStream).ReadToEnd());
            s_InfoStream.Dispose();
            return tag;
        }

        public static void DeleteSceneInfo(string path)
        {
            if (!File.Exists(path))
                return;
            FileStream stream = File.Open(path, FileMode.Open);
            Tag? scene = Parser.ParseXML(new StreamReader(stream).ReadToEnd());
            stream.Dispose();
            if (!scene.HasValue)
                return;
            DeleteSceneInfo(Guid.Parse(scene.Value.GetAttribute("Id")));
        }

        public static void DeleteSceneInfo(Guid sceneId)
        {
            if (!File.Exists($"{Path}Scenes\\scenes.osi"))
                return;
            FileStream stream = File.Open($"{Path}Scenes\\scenes.osi", FileMode.Open);
            Tag? sceneInfo = Parser.ParseXML(new StreamReader(stream).ReadToEnd());
            if (!sceneInfo.HasValue) 
                return;
            foreach (Tag scene in sceneInfo.Value.Childs)
            {
                if (Guid.Parse(scene.GetAttribute("Id")) == sceneId)
                {
                    sceneInfo.Value.Childs.Remove(scene);
                    stream.SetLength(0);
                    stream.Write(new UTF8Encoding().GetBytes(Parser.ToXML(sceneInfo.Value)));
                    stream.Flush();
                    break;
                }
            }
            stream.Dispose();
        }
    }
}

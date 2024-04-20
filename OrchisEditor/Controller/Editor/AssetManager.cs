using System;
using System.Collections.Generic;
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
    internal class AssetChangeEventArgs
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
        
        private AssetManager() { }

        public static void Init()
        {
            if (Project.IsLoaded)
            { 
                s_AssetsFolderPath = $"{Project.ProjectPath}Assets\\";
                s_Watcher = new FileSystemWatcher(s_AssetsFolderPath);
                s_Watcher.Created += OnWatcherCreated;
                s_Watcher.Deleted += OnWatcherDeleted;
                s_Watcher.Renamed += OnWatcherRenamed;
                s_Watcher.Changed += OnWatcherChanged;
                s_Watcher.EnableRaisingEvents = true;
            }
        }

        public static string Path { get { return s_AssetsFolderPath; } }

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
    }
}

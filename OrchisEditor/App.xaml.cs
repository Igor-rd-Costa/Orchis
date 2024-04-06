using Microsoft.VisualBasic.FileIO;
using OrchisEditor.Controller.Utils;
using System.Configuration;
using System.Data;
using System.IO;
using System.Text;
using System.Windows;

namespace OrchisEditor
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        const string m_CurrentVersion = "0.0.1";
        FileStream m_MetaDataStream;
        public App()
        {
            if (!File.Exists("./OrchisEditor.meta"))
            {
                m_MetaDataStream = File.Create("./OrchisEditor.meta");
                Tag editorMetaData = new();
                editorMetaData.Name = "OrchisEditor";
                editorMetaData.Attributes.Add(new TagAttribute("Version", m_CurrentVersion));
                editorMetaData.Attributes.Add(new TagAttribute("LastProjectPath", FileSystem.CurrentDirectory + "\\"));
                Tag openedProjects = new();
                openedProjects.Name = "RecentProjects";
                editorMetaData.Childs.Add(openedProjects);
                string xml = Parser.ToXML(editorMetaData);
                m_MetaDataStream.Write(new UTF8Encoding(true).GetBytes(xml));
                m_MetaDataStream.Flush();
            } 
            else {
                m_MetaDataStream = File.Open("./OrchisEditor.meta", FileMode.Open);
                string content = new StreamReader(m_MetaDataStream).ReadToEnd();
                Tag? tag = Parser.ParseXML(content);
                if (!tag.HasValue)
                {
                    // TODO handle parsing error
                } 
                else
                {
                    Tag metaData = tag.Value;
                    if (metaData.Name != "OrchisEditor")
                    {
                        // TODO handle invalid file
                        return;
                    }

                    for (int i = 0; i <  metaData.Attributes.Count; i++)
                    {
                        if (metaData.Attributes[i].Name == "Version" && metaData.Attributes[i].Value != m_CurrentVersion)
                        {
                            // TODO handle invalid file
                            break;
                        }
                    }
                }
            }

        }

        public Tag[] GetRecentProjects()
        {
            m_MetaDataStream.Seek(0, SeekOrigin.Begin);
            string content = new StreamReader(m_MetaDataStream).ReadToEnd();
            Tag? tag = Parser.ParseXML(content);

            if (tag.HasValue && tag.Value.Name == "OrchisEditor")
            {
                for (int i = 0; i < tag.Value.Childs.Count; i++)
                {
                    if (tag.Value.Childs[i].Name == "RecentProjects")
                    {
                        return tag.Value.Childs[i].Childs.ToArray();
                    }
                }
            }
            return [];
        }
        public void RegisterRecentProject(string name, string path)
        {
            m_MetaDataStream.Seek(0, SeekOrigin.Begin);
            string content = new StreamReader(m_MetaDataStream).ReadToEnd();
            Tag? tag = Parser.ParseXML(content);
            if (tag.HasValue && tag.Value.Name == "OrchisEditor")
            {
                for (int i = 0; i < tag.Value.Childs.Count; i++)
                {
                    if (tag.Value.Childs[i].Name == "RecentProjects")
                    {
                        Tag recentProject = tag.Value.Childs[i];
                        foreach (var child in recentProject.Childs)
                        {
                            if (child.GetAttribute("FilePath") == path)
                                return;
                        }

                        recentProject.Childs = recentProject.Childs.Prepend(new()
                        {
                            Name = name,
                            Attributes = [new TagAttribute("FilePath", path)],
                        }).ToList();
                        tag.Value.Childs[i] = recentProject;
                        m_MetaDataStream.SetLength(0);
                        string newContent = Parser.ToXML(tag.Value);
                        m_MetaDataStream.Write(new UTF8Encoding(true).GetBytes(newContent));
                        m_MetaDataStream.Flush();
                        break;
                    }
                }
            }

        }

        public void RemoveRecentProject(string path)
        {
            m_MetaDataStream.Seek(0, SeekOrigin.Begin);
            string content = new StreamReader(m_MetaDataStream).ReadToEnd();
            Tag? tag = Parser.ParseXML(content);

            if (tag.HasValue && tag.Value.Name == "OrchisEditor")
            {
                for (int i = 0; i < tag.Value.Childs.Count; i++)
                {
                    if (tag.Value.Childs[i].Name == "RecentProjects")
                    {
                        Tag recentProject = tag.Value.Childs[i];
                        foreach (var child in recentProject.Childs)
                        {
                            if (child.GetAttribute("FilePath") == path)
                            {
                                recentProject.Childs.Remove(child);
                                m_MetaDataStream.SetLength(0);
                                string newContent = Parser.ToXML(tag.Value);
                                m_MetaDataStream.Write(new UTF8Encoding(true).GetBytes(newContent));
                                m_MetaDataStream.Flush();
                                return;
                            }
                        }
                    }
                }
            }
        }
        public string GetLastProjectPath()
        {
            m_MetaDataStream.Seek(0, SeekOrigin.Begin);
            string content = new StreamReader(m_MetaDataStream).ReadToEnd();
            Tag? tag = Parser.ParseXML(content);
            if (tag.HasValue && tag.Value.Name == "OrchisEditor")
            {
                return tag.Value.GetAttribute("LastProjectPath");
            }
            return "";
        }
        public void SetLastProjectPath(string path)
        {
            m_MetaDataStream.Seek(0, SeekOrigin.Begin);
            string content = new StreamReader(m_MetaDataStream).ReadToEnd();
            Tag? tag = Parser.ParseXML(content);
            if (tag.HasValue && tag.Value.Name == "OrchisEditor")
            {
                tag.Value.SetAttribute("LastProjectPath", path);
                m_MetaDataStream.SetLength(0);
                string newContent = Parser.ToXML(tag.Value);
                m_MetaDataStream.Write(new UTF8Encoding(true).GetBytes(newContent));
                m_MetaDataStream.Flush();
            }
        }

        protected override void OnExit(ExitEventArgs e)
        {
            m_MetaDataStream.Flush();
            m_MetaDataStream.Dispose();
            base.OnExit(e);
        }
    }

}

using OrchisEditor.View.Editor;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace OrchisEditor.Controller.Editor
{
    public class Scene
    {
        public Scene(string name, int id) 
        {
            m_Name = name;
            m_Id = id;
            m_Entities = new();
        }

        public int ID
        {
            get { return m_Id; }
        }
        public string Name
        {
            get { return m_Name; }
            set { m_Name = value; }
        }

        public int GetEntityCount() { return m_Entities.Count; }
        public Entity GetEntity(int index) {  return m_Entities[index]; }

        public Entity? GetEntity(string name)
        {
            foreach (Entity entity in m_Entities)
            {
                if (entity.Name == name)
                    return entity;
            }
            return null;
        }
        public int AddEntity(string name = "NewEntity") 
        {
            int nameExt = 2;
            string newEntName = name;

            bool foundName = true;
            while (foundName)
            {
                foundName = false;
                foreach (Entity entity in m_Entities)
                {
                    if (entity.Name == newEntName)
                    {
                        newEntName = name + nameExt++;
                        foundName = true;
                        break;
                    }
                }
            }

            m_Entities.Add(new(newEntName));
            ((EditorWindow)Application.Current.MainWindow).ProjectOutliner.UpdateSceneGUI(this);
            Project.s_HasUnsavedChanges = true;
            return m_Entities.Count - 1; 
        }
        public void RemoveEntity(int index) { m_Entities.RemoveAt(index); }

        public void RemoveEntity(string name)
        {
            for (int i = 0; i < m_Entities.Count; i++)
            {
                if (m_Entities[i].Name == name)
                {
                    m_Entities.RemoveAt(i);
                    ((EditorWindow)Application.Current.MainWindow).ProjectOutliner.UpdateSceneGUI(this);
                }
            }
        }

        private string m_Name;
        private int m_Id;
        private readonly List<Entity> m_Entities;
    }
}

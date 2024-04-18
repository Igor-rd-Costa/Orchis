using OrchisEditor.Controller.Orchis;
using OrchisEditor.Controller.Utils;
using OrchisEditor.View.Editor;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Permissions;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace OrchisEditor.Controller.Editor
{
    public class Scene
    {
        private string m_Name;
        private Guid m_Id;
        private bool m_IsActive;
        private readonly List<Entity> m_Entities = [];

        public Guid Id
        {
            get { return m_Id; }
        }
        public string Name
        {
            get { return m_Name; }
            set { m_Name = value; }
        }
        public bool Active
        {
            get { return m_IsActive; }
            set { m_IsActive = value; }
        }
        public List<Entity> Entities
        {
            get { return m_Entities; }
        }
        public Scene(string name, bool isActive)
        {
            m_Name = name;
            m_IsActive = isActive;
            m_Id = OrchisInterface.OrchisSceneManagerCreateScene(Guid.Empty, isActive);
        }

        public Scene(string name, Guid id, bool isActive) 
        {
            m_Name = name;
            m_Id = OrchisInterface.OrchisSceneManagerCreateScene(id, isActive);
            m_IsActive = isActive;
        }

        public Entity? GetEntity(Guid id)
        {
            foreach (Entity entity in m_Entities)
            {
                if (entity.Id == id)
                    return entity;
            }
            return null;
        }
        public int AddEntity(Guid entityId, string name = "NewEntity") 
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
            if (entityId == Guid.Empty)
                m_Entities.Add(new(m_Id, newEntName));
            else
                m_Entities.Add(new(m_Id, newEntName, entityId));
            Project.RegisterChange();
            EditorWindow.GetProjectOutliner()?.UpdateSceneGUI(this);
            return m_Entities.Count - 1;
        }
        public void RemoveEntity(Guid id) 
        { 
            for (int i = 0; i < m_Entities.Count; i++)
            {
                if (m_Entities[i].Id == id)
                {
                    OrchisInterface.OrchisSceneRemoveEntity(m_Id, id);
                    m_Entities.RemoveAt(i);
                    Project.RegisterChange();
                    EditorWindow.GetProjectOutliner()?.UpdateSceneGUI(this);
                }
            }
        }

        public void RemoveEntity(string name)
        {
            for (int i = 0; i < m_Entities.Count; i++)
            {
                if (m_Entities[i].Name == name)
                {
                    m_Entities.RemoveAt(i);
                    EditorWindow.GetProjectOutliner()?.UpdateSceneGUI(this);
                }
            }
        }

        public bool LoadEntities(Tag scene)
        {
            Guid sceneId = Guid.Parse(scene.GetAttribute("Id"));
            foreach (Tag entity in scene.Childs)
            {
                if (entity.Name != "Entity")
                    continue;

                string entityName = entity.GetAttribute("Name");
                string entityId = entity.GetAttribute("Id");
                if (entityName == "")
                {
                    //TODO handle error, could not find entity name
                    return false;
                }
                Guid eId = Guid.Parse(entityId);
                m_Entities.Add(new(sceneId, entityName, eId));
                bool status = m_Entities[^1].LoadComponents(entity);
                if (!status)
                    return false;
            }
            return true;
        }

        public void DebugEntities()
        {
            Console.WriteLine($"Editor:\nScene {m_Id}: {m_Entities.Count} entities.");
            OrchisInterface.OrchisSceneDebugEntities(m_Id);
        }
    }
}

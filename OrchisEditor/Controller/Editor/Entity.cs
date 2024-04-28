using OrchisEditor.Controller.Editor.Components;
using OrchisEditor.Controller.Orchis;
using OrchisEditor.Controller.Utils;
using System;
using System.Collections.Generic;
using System.IO.Packaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrchisEditor.Controller.Editor
{
    public class Entity
    {
        private readonly Guid m_Id;
        private string m_Name;
        private readonly List<Component> m_Components = [];

        public Entity(string name, Guid entityId)
        {
            m_Name = name;
            m_Id = OrchisInterface.OrchisSceneAddEntity(entityId);
        }

        public void AddComponent(ComponentType type)
        {
            Guid id = OrchisInterface.OrchisEntityAddComponent(m_Id, type);
            if (id == Guid.Empty)
                return;
            m_Components.Add(new() { Id = id, Type = type });
            SceneManager.RegisterChange();
        }

        public void RemoveComponent(Guid componentId)
        {
            for (int i = 0; i < m_Components.Count; i++)
            {
                if (m_Components[i].Id == componentId)
                {
                    OrchisInterface.OrchisEntityRemoveComponent(m_Id, componentId);
                    m_Components.RemoveAt(i);
                    SceneManager.RegisterChange();
                }
            }
        }

        public bool LoadComponents(Tag entity)
        {
            Guid entityId = Guid.Parse(entity.GetAttribute("Id"));
            foreach (Tag component in entity.Childs)
            {
                if (component.Name != "Component")
                    continue;

                Guid componentId = Guid.Parse(component.GetAttribute("Id"));
                ComponentType componentType = Component.ParseType(component.GetAttribute("Type"));
                if (componentId == Guid.Empty || componentType == ComponentType.INVALID)
                {
                    //TODO handle error
                    return false;
                }
                m_Components.Add(new(entityId, componentId, componentType, component));
            }
            return true;
        }

        public string Name
        {
            get { return m_Name; }
            set { m_Name = value; }
        }
        public Guid Id
        {
            get { return m_Id; } 
        }
        public List<Component> Components
        {
            get { return m_Components; }
        }
    }
}

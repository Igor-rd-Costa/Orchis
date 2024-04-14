using OrchisEditor.Controller.Editor.Components;
using OrchisEditor.Controller.Orchis;
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
        private Guid m_Id;
        private string m_Name;
        private readonly List<Component> m_Components = [];

        public Entity(ref Guid sceneId, string name)
        {
            m_Name = name;
            m_Id = OrchisInterface.OrchisSceneAddEntity(sceneId, Guid.Empty);
        }

        public Entity(ref Guid sceneId, string name, Guid entityId)
        {
            m_Name = name;
            m_Id = OrchisInterface.OrchisSceneAddEntity(sceneId, entityId);
        }

        public void AddComponent(ref Guid sceneId, ComponentType type)
        {
            Guid id = OrchisInterface.OrchisEntityAddComponent(sceneId, m_Id, type);
            if (id == Guid.Empty)
                return;
            m_Components.Add(new() { Id = id, Type = type });
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
    }
}

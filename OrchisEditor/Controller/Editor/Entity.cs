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
        public Entity(string name) 
        {
            m_Name = name;
        }

        public string Name
        { 
            get { return m_Name; } 
            set { m_Name = value; }
        }

        private string m_Name;
    }
}

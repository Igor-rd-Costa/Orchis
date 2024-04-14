using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrchisEditor.Controller.Editor.Components
{
    public enum ComponentType
    {
        TRANSFORM
    }

    public struct Component
    {
        public Guid Id;
        public ComponentType Type;
    }
}

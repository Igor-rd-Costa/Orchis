using OrchisEditor.Controller.Orchis;
using OrchisEditor.Controller.Utils;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace OrchisEditor.Controller.Editor.Components
{
    public enum ComponentType
    {
        TRANSFORM, TEXTURE, MESH, INVALID
    }

    public class Component
    {
        public Guid Id;
        public ComponentType Type;

        public Component() { }
        public Component(Guid entityId, Guid componentId, ComponentType componentType, Tag component)
        {
            Type = componentType;

            Guid id = Guid.Empty;
            switch (componentType)
            {
                case ComponentType.TRANSFORM:
                {
                    string[] posString = component.GetAttribute("Position").Split(',');
                    string[] rotString = component.GetAttribute("Rotation").Split(',');
                    string[] sclString = component.GetAttribute("Scale").Split(',');
                    try
                    {
                        Vector3 pos = new(float.Parse(posString[0]), float.Parse(posString[1]), float.Parse(posString[2]));
                        Vector3 rot = new(float.Parse(rotString[0]), float.Parse(rotString[1]), float.Parse(rotString[2]));
                        Vector3 scl = new(float.Parse(sclString[0]), float.Parse(sclString[1]), float.Parse(sclString[2]));
                        id = OrchisInterface.OrchisEntityAddTransformComponent(entityId, componentId, pos, rot, scl);
                    } catch (Exception)
                    {
                        Id = Guid.Empty;
                    }
                } break;
                case ComponentType.MESH:
                {
                    MeshType meshType = ParseMeshType(component.GetAttribute("MeshType"));
                    Guid meshId = Guid.Parse(component.GetAttribute("MeshId"));
                    id = OrchisInterface.OrchisEntityAddMeshComponent(entityId, componentId, meshId, meshType);
                } break;
                default:
                {
                    Debug.Assert(false, "ComponentType not implemented.");
                } break;
            }
            Id = id;
        }

        public static ComponentType ParseType(string type)
        {
            if (type == ComponentType.TRANSFORM.ToString())
                return ComponentType.TRANSFORM;
            if (type == ComponentType.MESH.ToString()) 
                return ComponentType.MESH;

            return ComponentType.INVALID;
        }

        public static MeshType ParseMeshType(string type)
        {
            if (type == MeshType.STATIC.ToString())
                return MeshType.STATIC;

            return MeshType.STATIC;
        }
    }

    public struct TransformComponent
    {
        public Guid Id;
        public Vector3 Position;
        public Vector3 Rotation;
        public Vector3 Scale;
    }

    public enum MeshType
    {
        STATIC
    }
    public struct MeshComponent
    {
        public Guid Id;
        public Guid MeshId;
        public MeshType MeshType;
    }
}

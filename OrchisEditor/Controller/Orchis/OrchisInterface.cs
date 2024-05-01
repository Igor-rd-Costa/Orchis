using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Editor.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace OrchisEditor.Controller.Orchis
{
    public enum EditorEventType
    {
        MOUSE_MOVE
    };

    public struct EditorEventArgs
    {
        public EditorEventType type;
    };

    internal class OrchisInterface
    {
        //Application
        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisInit(IntPtr parentWindow);

        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisShutdown();

        [DllImport("OrchisRuntime.dll")]
        protected static extern IntPtr OrchisGetMainWindowHandle();

        //Renderer
        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisRendererSetActiveCamera(IntPtr camera);

        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisRendererSetDefaultShaders(string vertexPath, string fragPath);


        //SceneManager
        [DllImport("OrchisRuntime.dll")]
        public static extern Guid OrchisSceneManagerCreateScene(Guid sceneId);

        //Scene
        [DllImport("OrchisRuntime.dll")]
        public static extern Guid OrchisSceneAddEntity(Guid entityId);
        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisSceneRemoveEntity(Guid entityId);

        //Entity
        [DllImport("OrchisRuntime.dll")]
        public static extern Guid OrchisEntityAddComponent(Guid entityId, ComponentType componentType);
        [DllImport("OrchisRuntime.dll")]
        public static extern Guid OrchisEntityAddTransformComponent(Guid entityId, Guid componentId, Vector3 position, Vector3 rotation, Vector3 scale);
        [DllImport("OrchisRuntime.dll")]
        public static extern Guid OrchisEntityAddMeshComponent(Guid entityId, Guid componentId, Guid meshId, MeshType type);
        [DllImport("OrchisRuntime.dll")]
        public static extern Guid OrchisEntityAddTextureComponent(Guid entityId, Guid componentId, Guid textureId);
        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisEntityRemoveComponent(Guid entityId, Guid componentId);


        //Component Manager
        [DllImport("OrchisRuntime.dll")]
        public static extern TransformComponent OrchisComponentManagerGetTransformComponent(Guid componentId);
        [DllImport("OrchisRuntime.dll")]
        public static extern MeshComponent OrchisComponentManagerGetMeshComponent(Guid componentId);
        [DllImport("OrchisRuntime.dll")]
        public static extern TextureComponent OrchisComponentManagerGetTextureComponent(Guid componentId);

        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisComponentManagerTransformComponentSetPosition(Guid componentId, Vector3 position);
        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisComponentManagerTransformComponentSetRotation(Guid componentId, Vector3 rotation);
        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisComponentManagerTransformComponentSetScale(Guid componentId, Vector3 scale);

        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisComponentManagerUpdateMeshComponentMeshId(Guid componentId, Guid meshId);
        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisComponentManagerUpdateMeshComponentMeshType(Guid componentId, MeshType meshType);

        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisComponentManagerUpdateTextureComponentTextureId(Guid componentId, Guid textureId);

        protected delegate void EditorEventCallback(EditorEventArgs e);
        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisEditorRegisterEventCallback(EditorEventCallback callback);


        //Events
        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisEventsRegisterKeyDownEvent(OrchisKey key);
        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisEventsRegisterKeyUpEvent(OrchisKey key);


        //FS
        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisFileSystemMount(string path, string vPath);

        //Main Window
        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisMainWindowUpdateRect();

    }
}

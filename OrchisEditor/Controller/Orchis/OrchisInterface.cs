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
        MOUSE_MOVE, CAMERA_MOVE
    };

    public struct EditorMouseMoveEventArgs
    {
        
    }
    public struct EditorCameraMoveEventArgs
    {
        public EditorCameraData Data;
    }
    public struct EditorCameraData
    {
        public Vector3 Position;
        public Vector3 LookAt;
        public float Speed;
        public float FieldOfView;
        public float Yaw;
        public float Pitch;
    };

    internal class OrchisInterface
    {
        //Application
        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisInit(IntPtr parentWindow, EditorCameraData data);

        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisShutdown();

        [DllImport("OrchisRuntime.dll")]
        protected static extern IntPtr OrchisGetMainWindowHandle();


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

        
        //Editor
        public delegate void EditorMouseMoveEventCallback(EditorMouseMoveEventArgs e);
        public delegate void EditorCameraMoveEventCallback(EditorCameraMoveEventArgs e);
        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisEditorRegisterMouseMoveEventCallback(EditorMouseMoveEventCallback callback);
        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisEditorRegisterCameraMoveEventCallback(EditorCameraMoveEventCallback callback);
        [DllImport("OrchisRuntime.dll")]
        protected static extern Vector3 OrchisEditorGetEditorCameraPosition();
        [DllImport("OrchisRuntime.dll")]
        protected static extern Vector3 OrchisEditorGetEditorCameraLookAt();
        [DllImport("OrchisRuntime.dll")]
        protected static extern EditorCameraData OrchisEditorGetEditorCameraData();
        
        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisEditorSetEditorCameraData(EditorCameraData data);
        

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

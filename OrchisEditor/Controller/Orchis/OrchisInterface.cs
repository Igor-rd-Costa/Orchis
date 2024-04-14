using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Editor.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace OrchisEditor.Controller.Orchis
{
    internal class OrchisInterface
    {
        [DllImport("OrchisRuntime.dll")]
        public static extern void TestTest();

        //Application
        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisInit(IntPtr parentWindow);

        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisShutdown();

        [DllImport("OrchisRuntime.dll")]
        protected static extern IntPtr OrchisGetMainWindowHandle();


        //Camera
        [DllImport("OrchisRuntime.dll")]
        protected static extern IntPtr OrchisCameraCreate();

        [DllImport("OrchisRuntime.dll")]
        protected static extern void OrchisCameraDelete(IntPtr camera);


        //Renderer
        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisRendererSetActiveCamera(IntPtr camera);

        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisRendererSetDefaultShaders(string vertexPath, string fragPath);


        //SceneManager
        [DllImport("OrchisRuntime.dll")]
        public static extern Guid OrchisSceneManagerCreateScene(Guid sceneId, bool makeActive = false);
        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisSceneManagerDeleteScene(Guid sceneId);  
        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisSceneManagerLoadScene();
        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisSceneManagerUnloadScene();


        //Scene
        [DllImport("OrchisRuntime.dll")]
        public static extern Guid OrchisSceneAddEntity(Guid sceneId, Guid entityId);


        //Entity
        [DllImport("OrchisRuntime.dll")]
        public static extern Guid OrchisEntityAddComponent(Guid sceneId, Guid entityId, ComponentType type);

    }
}

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
        //Application
        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisInit(IntPtr parentWindow);

        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisShutdown();

        [DllImport("OrchisRuntime.dll")]
        public static extern IntPtr OrchisGetMainWindowHandle();


        //Camera
        [DllImport("OrchisRuntime.dll")]
        public static extern IntPtr OrchisCameraCreate();

        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisCameraDelete(IntPtr camera);


        //Renderer
        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisRendererSetActiveCamera(IntPtr camera);

        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisRendererSetDefaultShaders(string vertexPath, string fragPath);


        //Scene
        [DllImport("OrchisRuntime.dll")]
        public static extern void OrchisSceneCreate(string name);
    }
}

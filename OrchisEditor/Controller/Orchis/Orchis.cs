using OrchisEditor.Controller.Editor;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrchisEditor.Controller.Orchis
{
    internal class Engine : OrchisInterface
    {
        private Engine() { }

        public static void Init(IntPtr parentWindow)
        {
            OrchisInit(parentWindow);
        }

        public static void Shutdown()
        {
            OrchisShutdown();
        }

        public static IntPtr GetMainWindowHandle()
        {
            return OrchisGetMainWindowHandle();
        }

        internal class Camera
        {
            private Camera() { }

            public static IntPtr Create()
            {
                return OrchisCameraCreate();
            }

            public static void Delete(IntPtr camera)
            {
                OrchisCameraDelete(camera);
            }
        }

        internal class Renderer
        {
            private Renderer() { }
        }
    }
}

using OrchisEditor.Controller.Orchis;
using System;
using System.Runtime.InteropServices;
using System.Windows.Interop;

namespace OrchisEditor.Controller
{
    class WindowHost : HwndHost
    {
        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            OrchisInterface.OrchisRendererSetDefaultShaders(
                "../../../Assets/Shaders/bin/vert.spv",
                "../../../Assets/Shaders/bin/frag.spv"
                );

            OrchisInterface.OrchisInit(hwndParent.Handle);
            IntPtr windowHandle = OrchisInterface.OrchisGetMainWindowHandle();
            return new HandleRef(this, windowHandle);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            OrchisInterface.OrchisShutdown();
        }
    }
}

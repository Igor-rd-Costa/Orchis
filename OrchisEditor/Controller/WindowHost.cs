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
            OrchisInterface.Application.OrchisInit(hwndParent.Handle);
            IntPtr windowHandle = OrchisInterface.Application.OrchisGetMainWindowHandle();
            return new HandleRef(this, windowHandle);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            OrchisInterface.Application.OrchisShutdown();
        }
    }
}


using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Orchis;
using OrchisEditor.View.Editor;
using System;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Input;
using System.Windows.Interop;

namespace OrchisEditor.Controller
{


    class WindowHost : HwndHost
    {
        private IntPtr m_Hwnd;
        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            Engine.Init(hwndParent.Handle, Project.Settings.CameraData);
            m_Hwnd = Engine.GetMainWindowHandle();
            return new HandleRef(this, m_Hwnd);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            Engine.Shutdown();
        }
    }
}

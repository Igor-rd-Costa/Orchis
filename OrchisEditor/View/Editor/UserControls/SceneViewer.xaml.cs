using OrchisEditor.Controller;
using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Orchis;
using System;
using System.Runtime.InteropServices;
using System.Windows.Controls;

namespace OrchisEditor.View.Editor.UserControls
{
    public partial class SceneViewer : UserControl
    {
        private WindowHost m_WindowHost;
        public SceneViewer()
        {
            InitializeComponent();
            m_WindowHost = new WindowHost();

            if (Project.IsLoaded)
            {
                Scene? scene = Project.ActiveScene;
                if (scene != null) 
                {
                    OrchisInterface.OrchisSceneCreate(scene.Name);
                }
            }
        }

        public void Attach()
        {
            SceneViewWrapper.Child = m_WindowHost;
        }

        private IntPtr projectHandle = IntPtr.Zero;
    }
}

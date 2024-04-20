using OrchisEditor.Controller;
using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Orchis;
using System;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Controls;

namespace OrchisEditor.View.Editor.UserControls
{
    public partial class SceneViewer : UserControl
    {
        private WindowHost? m_WindowHost;
        public SceneViewer()
        {
            InitializeComponent();
            if (Project.IsLoaded)
            {
                m_WindowHost = new WindowHost();
            }
        }

        public void Attach()
        {
            SceneViewWrapper.Child = m_WindowHost;
        }
    }
}

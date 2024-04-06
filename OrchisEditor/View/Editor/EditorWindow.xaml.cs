using System;
using System.Windows;
using OrchisEditor.Controller.Editor;
using OrchisEditor.View.ProjectSelector;

namespace OrchisEditor.View.Editor
{
    public partial class EditorWindow : Window
    {
        public EditorWindow()
        {
            m_ProjectSelector = new ProjectSelectorWindow();
            if (m_ProjectSelector.ShowDialog() == false)
            {
                Close();
            }
            InitializeComponent();
            Title = Project.Name + " - " + Title;
            SystemMenu.Window = this;
            SceneRenderer.Attach();
        }

        ProjectSelectorWindow m_ProjectSelector;

        private void Window_MouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (e.GetPosition(this).Y < WindowGrid.RowDefinitions[0].Height.Value)
            {
                DragMove();
            }
        }
    }
}

using System;
using System.Windows;
using OrchisEditor.Controller.Editor;

namespace OrchisEditor.View.ProjectSelector
{
    public partial class ProjectSelectorWindow : Window
    {
        public ProjectSelectorWindow()
        {
            InitializeComponent();
            SystemMenu.Window = this;
        }

        private void CreateProjectBtn_Click(object sender, RoutedEventArgs e)
        {
            ProjectCreatorWindow projectCreatorWindow = new ProjectCreatorWindow();
            if (projectCreatorWindow.ShowDialog() == true)
            {
                DialogResult = true;
                Close();
            }
        }

        private void Window_MouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (e.GetPosition(this).Y < WindowGrid.RowDefinitions[0].Height.Value)
            {
                DragMove();
            }
        }

        private void ReloadBtn_Click(object sender, RoutedEventArgs e)
        {
            ProjectList.Reload();
        }

        private void Window_StateChanged(object sender, EventArgs e)
        {
            Console.WriteLine("State Changed to: " + e.ToString());
        }
    }
}

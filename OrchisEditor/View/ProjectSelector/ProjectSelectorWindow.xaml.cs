using System;
using System.Windows;
using Microsoft.Win32;
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

        private void OpenBtn_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.DefaultExt = "orcproj";

            bool? isValid = false;
            string file = "";
            do
            {
                bool? status = dialog.ShowDialog();
                if (status.HasValue && status.Value)
                {
                    file = dialog.FileName;
                    Console.WriteLine("File: " + file);
                    int index = file.LastIndexOf('.');
                    if (file.Substring(index, file.Length - index) == ".orcproj")
                        isValid = true;
                }
                else
                {
                    isValid = null;
                }
            } while (isValid.HasValue && !isValid.Value);
            Project.Load(file);
        }

        private void Window_StateChanged(object sender, EventArgs e)
        {
            Console.WriteLine("State Changed to: " + e.ToString());
        }
    }
}

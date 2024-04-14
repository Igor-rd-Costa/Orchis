using Microsoft.VisualBasic.FileIO;
using Microsoft.Win32;
using OrchisEditor.Controller.Editor;
using OrchisEditor.View.UserControls;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media.Imaging;

namespace OrchisEditor.View.ProjectSelector
{
    public partial class ProjectCreatorWindow : Window
    {
        public ProjectCreatorWindow()
        {
            DataContext = this;
            InitializeComponent();
            SystemMenu.Window = this;
            ProjectPathInput.DataContext = this;
            ProjectPathInput.Text = ((App)Application.Current).GetLastProjectPath();
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.GetPosition(this).Y < WindowGrid.RowDefinitions[0].Height.Value)
            {
                DragMove();
            }
        }

        private void ProjectTemplates_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (ProjectTemplates.SelectedItem != null)
            {
                ListBoxItem item = (ListBoxItem)ProjectTemplates.SelectedItem;
                switch (item.Name)
                {
                    case "BlankProject":
                    {
                        if (ProjectTemplateName != null)
                        {
                            ProjectTemplateName.Text = "Blank Project";
                            ProjectTemplateDescription.Text = "A blank project";
                            BitmapImage bitmapImage = new();
                            bitmapImage.BeginInit();
                            bitmapImage.UriSource = new Uri("/View/Images/ProjectPreviewTest.png", UriKind.Relative);
                            bitmapImage.EndInit();
                            ProjectTemplateImage.Source = bitmapImage;
                            CreateProjectBtn.IsEnabled = true;
                        }
                    }
                    break;
                    case "OtherProject":
                    {
                        if (ProjectTemplateName != null)
                        {
                            ProjectTemplateName.Text = "Other Project";
                            ProjectTemplateDescription.Text = "Project description goes here!";
                            BitmapImage bitmapImage = new();
                            bitmapImage.BeginInit();
                            bitmapImage.UriSource = new Uri("/View/Images/ImageTest.png", UriKind.Relative);
                            bitmapImage.EndInit();
                            ProjectTemplateImage.Source = bitmapImage;
                            CreateProjectBtn.IsEnabled = false;
                        }
                    }
                    break;
                }
            }
        }

        private void ProjectName_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (String.IsNullOrEmpty(ProjectName.Text))
                CreateProjectBtn.IsEnabled = false;
            else CreateProjectBtn.IsEnabled = true;
        }

        private void CreateProjectBtn_Click(object sender, RoutedEventArgs e)
        {
            if (ProjectTemplates.SelectedItem != null)
            {
                ListBoxItem item = (ListBoxItem)ProjectTemplates.SelectedItem;
                if (item.Name == "BlankProject")
                {
                    string projectPath = ProjectPathInput.Text;
                    if (FileSystem.DirectoryExists(projectPath+ProjectName.Text))
                    {
                        // TODO handle error. Invalid directory
                        OrchisDialog dialog = new OrchisDialog();
                        dialog.SetDialogType(OrchisDialogType.OK);
                        dialog.ShowMessage($"A folder called {ProjectName.Text} already exists in the specified path.", "Error");
                        return;
                    }

                    DialogResult = true;
                    Project.Create(Project.ProjectTemplate.BLANK, ProjectName.Text, projectPath);
                    Close();
                }
            }
        }

        private void SelectProjectPath(object sender, RoutedEventArgs e)
        {
            var dirDialog = new OpenFolderDialog();
            dirDialog.Multiselect = false;
            dirDialog.ShowDialog();
            ProjectPathInput.Text = dirDialog.FolderName + "\\";
        }
    }
}

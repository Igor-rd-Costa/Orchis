using Microsoft.VisualBasic.FileIO;
using OrchisEditor.Controller.Utils;
using OrchisEditor.View.UserControls;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace OrchisEditor.View.ProjectSelector.UserControls
{
    public partial class ProjectBrowser : UserControl
    {
        public ProjectBrowser()
        {
            InitializeComponent();
            BuildProjectList();
        }

        void BuildProjectList()
        {
                var recentProjects = ((App)Application.Current).GetRecentProjects();
                foreach (Tag project in recentProjects)
                {
                    string path = project.GetAttribute("FilePath");
                    ProjectList.Items.Add(MakeProjectListItem(project.Name, path));
                }
        }

        private ListBoxItem MakeProjectListItem(string name, string filePath)
        {
            ListBoxItem item = new()
            {
                Height = 60,
                Padding = new(0),
                HorizontalAlignment = HorizontalAlignment.Stretch,
                HorizontalContentAlignment = HorizontalAlignment.Stretch
            };
            Grid grid = new()
            {
                Height = 58,
                HorizontalAlignment = HorizontalAlignment.Stretch,
                VerticalAlignment = VerticalAlignment.Stretch,
            };
            grid.ColumnDefinitions.Add(new ColumnDefinition());
            grid.ColumnDefinitions.Add(new ColumnDefinition());
            grid.ColumnDefinitions.Add(new ColumnDefinition());
            grid.ColumnDefinitions[0].Width = new GridLength(31);
            grid.ColumnDefinitions[1].Width = new GridLength(1, GridUnitType.Star);
            grid.ColumnDefinitions[2].Width = new GridLength(130);

            grid.RowDefinitions.Add(new RowDefinition());
            grid.RowDefinitions.Add(new RowDefinition());
            grid.RowDefinitions[0].Height = new(31);
            grid.RowDefinitions[1].Height = new(27);

            BitmapImage bitmapImage = new();
            bitmapImage.BeginInit();
            bitmapImage.UriSource = new Uri("/View/Images/ImageTest.png", UriKind.Relative);
            bitmapImage.EndInit();
            Image image = new()
            {
                Height = 22, 
                Width = 22, 
                Source = bitmapImage,
                VerticalAlignment = VerticalAlignment.Bottom,
                HorizontalAlignment= HorizontalAlignment.Right,

            };
            Grid.SetColumn(image, 0);
            grid.Children.Add(image);
            TextBlock projectName = new()
            {
                Text = name,
                Foreground = Brushes.White,
                VerticalAlignment = VerticalAlignment.Center,
                Padding = new Thickness(5, 5, 0, 0),
                TextTrimming = TextTrimming.CharacterEllipsis
            };
            TextBlock projectDate = new()
            {
                Text = "9/23/2023 11:26 AM", // TODO Add date
                Foreground = Brushes.White,
                VerticalAlignment = VerticalAlignment.Center,
                HorizontalAlignment = HorizontalAlignment.Stretch,
                Padding = new Thickness(5, 5, 0, 0),
            };
            TextBlock projectPath = new()
            {
                Text = filePath,
                Foreground = Brushes.White,
                VerticalAlignment = VerticalAlignment.Center,
                Padding = new Thickness(5, 0, 0, 0),
                TextTrimming = TextTrimming.CharacterEllipsis
            };

            Grid.SetColumn(projectName, 1);
            Grid.SetColumn(projectDate, 2);

            Grid.SetColumn(projectPath, 1);
            Grid.SetColumnSpan(projectPath, 2);
            Grid.SetRow(projectPath, 1);

            grid.Children.Add(projectName);
            grid.Children.Add(projectDate);
            grid.Children.Add(projectPath); 

            item.Content = grid;
            return item;
        }

        public void Reload()
        {
            ProjectList.Items.Clear();
            BuildProjectList();
        }

        private void ProjectList_MouseDoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (ProjectList.SelectedItem != null) 
            {
                string projectPath = ((TextBlock)((Grid)((ListBoxItem)ProjectList.SelectedItem).Content).Children[3]).Text;
                if (!FileSystem.FileExists(projectPath))
                {
                    var errorDialog = new OrchisErrorDialog();
                    bool? remove = errorDialog.ShowError("The project file could not be found.\nDo you wish to remove it from the list?");
                    if (remove.HasValue && remove.Value)
                    {
                        ((App)Application.Current).RemoveRecentProject(projectPath);
                        ProjectList.Items.Remove(ProjectList.SelectedItem);
                    }
                    return;
                }
                foreach (Window window in Application.Current.Windows)
                {
                    if (window.GetType().Equals(typeof(ProjectSelectorWindow)))
                    {
                        if (OrchisEditor.Controller.Editor.Project.Load(projectPath))
                        {
                            window.DialogResult = true;
                            break;
                        }
                    }
                }
            }
        }
    }
}

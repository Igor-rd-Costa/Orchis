using Microsoft.Win32;
using OrchisEditor.Controller.Editor;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace OrchisEditor.View.Editor.UserControls
{
    /// <summary>
    /// Interaction logic for MenuBar.xaml
    /// </summary>
    public partial class MenuBar : UserControl
    {
        public MenuBar()
        {
            InitializeComponent();
        }

        private void OnExit(object sender, MouseButtonEventArgs e)
        {
            if (Project.OnAppClose())
            {
                Application.Current.Shutdown();
            }
        }

        private void NewScene(object sender, MouseButtonEventArgs e)
        {
            SceneManager.CreateScene();
        }

        private void LoadScene(object sender, MouseButtonEventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.AddExtension = true;
            dialog.Multiselect = false;
            dialog.DefaultDirectory = $"{AssetManager.Path}\\Scenes";
            dialog.DefaultExt = "osn";
            bool? status = dialog.ShowDialog();
            if (status.HasValue && status.Value)
            {
                SceneManager.LoadScene(dialog.FileName);
                Project.RegisterChange();
            }
        }
    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
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

namespace OrchisEditor.View.Editor.UserControls.OutlinerComponents
{
    /// <summary>
    /// Interaction logic for ProjectHeader.xaml
    /// </summary>
    public partial class ProjectHeader : UserControl, INotifyPropertyChanged
    {
        private bool m_Selected = false;

        public ProjectHeader()
        {
            DataContext = this;
            InitializeComponent();
            m_ProjectName = "";
            EditorWindow.OnClick(OnGlobalClick);
        }

        public ProjectHeader(string projectName)
        {
            DataContext = this;
            InitializeComponent();
            m_ProjectName = projectName;
            EditorWindow.OnClick(OnGlobalClick);
        }

        private string m_ProjectName;

        public bool Selected
        {
            get { return m_Selected; }
            set
            {
                m_Selected = value;
                if (m_Selected)
                {
                    Wrapper.Background = (SolidColorBrush)FindResource("GrayBrush");
                }
                else
                {
                    Wrapper.Background = Brushes.Transparent;
                }
            }
        }
        public event PropertyChangedEventHandler? PropertyChanged;

        public string ProjectName
        {
            get { return m_ProjectName; }
            set 
            { 
                m_ProjectName = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ProjectName)));
            }
        }

        private void Wrapper_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Console.WriteLine("Click!");
            Selected = true;
            e.Handled = true;
        }

        private void OnGlobalClick(object sender, MouseButtonEventArgs e)
        {
            if (Selected)
            {
                Selected = false;
                if (EditorWindow.GetPropertyPanel()?.SelectedItem is ProjectHeader)
                {
                    EditorWindow.GetPropertyPanel()?.LoadProperties(null);
                }
            }
        }

        private void Wrapper_MouseEnter(object sender, MouseEventArgs e)
        {
            if (!m_Selected) { }
            //Wrapper.Background =
        }

        private void Wrapper_MouseLeave(object sender, MouseEventArgs e)
        {
            if (!m_Selected)
                Wrapper.Background = Brushes.Transparent;
        }
    }
}

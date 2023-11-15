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
        public ProjectHeader()
        {
            DataContext = this;
            InitializeComponent();
        }

        public ProjectHeader(string projectName)
        {
            DataContext = this;
            InitializeComponent();
            ProjectName = projectName;
        }

        private string m_ProjectName;

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

    }
}

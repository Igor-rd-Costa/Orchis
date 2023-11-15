using System.ComponentModel;
using System.Windows.Controls;

namespace OrchisEditor.View.ProjectSelector.UserControls
{
    public partial class ProjectBlock : UserControl, INotifyPropertyChanged
    {
        public ProjectBlock()
        {
            DataContext = this;
            InitializeComponent();
            ProjectName = "Project";
        }

        public ProjectBlock(string name)
        {
            DataContext = this;
            InitializeComponent();
            ProjectName = name;
        }

        public string ProjectName
        {
            get { return m_ProjectName; }
            set 
            { 
                m_ProjectName = value; 
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Name)));
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        private string m_ProjectName = "Project";
    }
}

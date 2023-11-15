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

namespace OrchisEditor.View.Editor.UserControls.WindowComponents
{
    /// <summary>
    /// Interaction logic for SysMenu.xaml
    /// </summary>
    public partial class SysMenu : UserControl
    {
        public SysMenu()
        {
            DataContext = this;
            InitializeComponent();
        }




        private void MinimizeBtn_Click(object sender, RoutedEventArgs e)
        {
            if (m_Window != null) 
                m_Window.WindowState = WindowState.Minimized;
        }

        private void MaximizeBtn_Click(object sender, RoutedEventArgs e)
        {
            if (m_Window != null)
            {
                if (m_Window.WindowState == WindowState.Maximized)
                    m_Window.WindowState = WindowState.Normal;
                else m_Window.WindowState = WindowState.Maximized;
            }
        }

        private void CloseBtn_Click(object sender, RoutedEventArgs e)
        {
            m_Window?.Close();
        }

        public Window Window
        {
            set { m_Window = value; }
        }

        private Window? m_Window;
    }
}

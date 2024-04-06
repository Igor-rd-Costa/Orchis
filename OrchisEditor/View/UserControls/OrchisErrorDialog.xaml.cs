using OrchisEditor.View.Editor.UserControls.WindowComponents;
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
using System.Windows.Shapes;
using Windows.UI;

namespace OrchisEditor.View.UserControls
{
    /// <summary>
    /// Interaction logic for OrchisErrorDialog.xaml
    /// </summary>
    public partial class OrchisErrorDialog : Window
    {
        public OrchisErrorDialog()
        {
            InitializeComponent();
            SystemMenu.Window = this;
            DataContext = this;
            SystemMenu.SetMode(SysMenuMode.CLOSE_ONLY);
        }

        public bool? ShowError(string message)
        {
            ErrorTextBlock.Text = message;
            return ShowDialog();
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.GetPosition(this).Y < WindowGrid.RowDefinitions[0].Height.Value)
            {
                DragMove();
            }
        }

        private void YesButton_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
            Close();
        }

        private void NoButton_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
            Close();
        }
    }
}

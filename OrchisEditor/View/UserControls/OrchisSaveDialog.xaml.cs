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
    /// Interaction logic for OrchisSaveDialog.xaml
    /// </summary>
    /// 
    public enum SaveDialogResult
    {
        SAVE, DONT_SAVE, CANCEL
    }
    public partial class OrchisSaveDialog : Window
    {
        private SaveDialogResult m_Result = SaveDialogResult.CANCEL;

        public SaveDialogResult Result
        {
            get { return m_Result; }
        }
        public OrchisSaveDialog()
        {
            InitializeComponent();
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
            m_Result = SaveDialogResult.SAVE;
            DialogResult = true;
        }
        private void NoButton_Click(object sender, RoutedEventArgs e)
        {
            m_Result = SaveDialogResult.DONT_SAVE;
            DialogResult= true;
        }
        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            m_Result = SaveDialogResult.CANCEL;
            DialogResult = true;
        }
    }
}

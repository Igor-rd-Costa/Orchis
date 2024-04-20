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

namespace OrchisEditor.View.Editor.UserControls.ToolsPanelComponents.AssetManagerComponents.AssetImportForm
{
    /// <summary>
    /// Interaction logic for AssetImportForm.xaml
    /// </summary>
    public partial class AssetImportForm : Window
    {
        public AssetImportForm()
        {
            InitializeComponent();
            SystemMenu.Window = this;
            SystemMenu.SetMode(SysMenuMode.CLOSE_ONLY);
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.GetPosition(this).Y < WindowGrid.RowDefinitions[0].Height.Value)
            {
                DragMove();
                return;
            }
        }
    }
}

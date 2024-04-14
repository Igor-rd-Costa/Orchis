using OrchisEditor.View.Editor.UserControls.OutlinerComponents;
using OrchisEditor.View.Editor.UserControls.PropertyPanelComponents;
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
    /// Interaction logic for PropertyPanel.xaml
    /// </summary>
    public partial class PropertyPanel : UserControl
    {
        public PropertyPanel()
        {
            InitializeComponent();
        }

        public void LoadProperties(OutlinerTreeItem treeItem)
        {
            BitmapImage bitmap = new BitmapImage();
            bitmap.BeginInit();
            bitmap.UriSource = new Uri("/View/Images/ImageTest.png", UriKind.Relative);
            bitmap.EndInit();
            //Icon.Source = bitmap;

            NameField.Text = treeItem.ItemName;
            switch (treeItem.Type)
            {
                case ItemType.ITEM_TYPE_SCENE:
                {
                        TypeField.Text = "Scene";
                } break;
                case ItemType.ITEM_TYPE_ENTITY: 
                {
                        TypeField.Text = "Entity";
                } break;
            }
        }





        private void TreeViewItem_BlockSelect(object sender, RoutedEventArgs e)
        {
            TreeViewItem item = (TreeViewItem)sender;
            if (item != null)
            {
                item.IsSelected = false;
                Console.WriteLine("Canceled Selection!");
            }
        }
    }
}

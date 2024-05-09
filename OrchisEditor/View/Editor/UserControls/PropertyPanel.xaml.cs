using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Editor.Components;
using OrchisEditor.Controller.Orchis;
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
        private object? m_Item = null;

        public object? SelectedItem
        {
            get { return m_Item; }
        }
        public PropertyPanel()
        {
            InitializeComponent();
            DataContext = this;
            Console.WriteLine("Properties init!");
        }

        public void LoadProperties(OutlinerTreeItem? treeItem)
        {
            if (treeItem == null)
            {
                m_Item = null;
                UpdateGUI();
                return;
            }

            //BitmapImage bitmap = new BitmapImage();
            //bitmap.BeginInit();
            //bitmap.UriSource = new Uri("/View/Images/ImageTest.png", UriKind.Relative);
            //bitmap.EndInit();
            //Icon.Source = bitmap;

            switch (treeItem.Type)
            {
                case ItemType.ITEM_TYPE_SCENE:
                {
                    m_Item = SceneManager.Scene;
                } break;
                case ItemType.ITEM_TYPE_ENTITY: 
                {
                    Entity? entity = SceneManager.Scene?.GetEntity(treeItem.Id);
                    m_Item = entity;
                } break;
            }
            UpdateGUI();
        }

        public void UpdateGUI()
        {
            Components.Children.Clear();
            if (m_Item == null)
            {
                ItemInfo.Visibility = Visibility.Hidden;
                return;
            }
            ItemInfo.Visibility = Visibility.Visible;
            if (m_Item is Scene scene)
            {
                NameField.Text = scene.Name;
                TypeField.Text = "Scene";
                Components.Children.Add(new EditorCameraComponentItem());
            }
            if (m_Item is Entity entity)
            {
                NameField.Text = entity.Name;
                TypeField.Text = "Entity";
                foreach (Component component in entity.Components)
                {
                    Components.Children.Add(GetComponent(component));
                }
            }
        }

        private UIElement GetComponent(Component component)
        {
            switch (component.Type)
            {
                case ComponentType.TRANSFORM:
                {
                    TransformComponent tc = OrchisInterface.OrchisComponentManagerGetTransformComponent(component.Id);
                    return new TransformComponentItem(component.Id, tc.Position, tc.Rotation, tc.Scale);
                } 
                case ComponentType.MESH:
                {
                    MeshComponent mc = OrchisInterface.OrchisComponentManagerGetMeshComponent(component.Id);
                    return new MeshComponentItem(component.Id, mc.MeshId, mc.MeshType);
                }
                case ComponentType.TEXTURE:
                {
                    TextureComponent tc = OrchisInterface.OrchisComponentManagerGetTextureComponent(component.Id);
                    return new TextureComponentItem(tc.Id, tc.TextureId);
                }
                default: return new TransformComponentItem(component.Id);
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

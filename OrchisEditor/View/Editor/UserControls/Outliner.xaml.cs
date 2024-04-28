using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Orchis;
using OrchisEditor.View.Editor.UserControls.OutlinerComponents;
using OrchisEditor.View.Editor.UserControls.OutlinerComponents.ContextMenus;
using OrchisEditor.View.UserControls;
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace OrchisEditor.View.Editor.UserControls
{
    public partial class Outliner : UserControl
    {
        public Outliner()
        {
            InitializeComponent();
            DataContext = this;
            Console.WriteLine("Outliner init!");
            if (Project.IsLoaded)
            {
                ProjectItem.Visibility = Visibility.Visible;
                ProjectItem.ProjectName = Project.Name;
                ProjectItem.ContextMenu = new ProjectContextMenu();
                SceneHierarchy.ContextMenu = new ProjectContextMenu();
                Scene? scene = SceneManager.Scene;
                if (scene == null)
                    return;
                SceneTreeView.Items.Add(new OutlinerTreeItem(scene.Id, scene.Name, "/View/Images/ImageTest.png", ItemType.ITEM_TYPE_SCENE));
                ((OutlinerTreeItem)SceneTreeView.Items[^1]).IsExpanded = true;
                foreach (Entity entity in scene.Entities)
                {
                    ((OutlinerTreeItem)SceneTreeView.Items[^1]).Items.Add(new OutlinerTreeItem(entity.Id, entity.Name, "./View/Images/MinimizeIcon.png", ItemType.ITEM_TYPE_ENTITY));
                }
            }
        }

        public void UpdateGUI()
        {
            //TODO not recreate the whole tree here
            SceneTreeView.Items.Clear();
            Scene? scene = SceneManager.Scene;
            if (scene == null)
                return;
            int index = SceneTreeView.Items.Add(new OutlinerTreeItem(scene.Id, scene.Name, "/View/Images/ImageTest.png", ItemType.ITEM_TYPE_SCENE));
            ((OutlinerTreeItem)SceneTreeView.Items[index]).IsExpanded = true;
            foreach (Entity entity in scene.Entities)
            {
                ((OutlinerTreeItem)SceneTreeView.Items[index]).Items.Add(new OutlinerTreeItem(entity.Id, entity.Name, "/View/Images/MinimizeIcon.png", ItemType.ITEM_TYPE_ENTITY));
            }
        }

        private void SceneTreeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (e.NewValue != null) 
            {
                OutlinerTreeItem treeItem = (OutlinerTreeItem)e.NewValue;
                ((EditorWindow)Application.Current.MainWindow).PropertiesPanel.LoadProperties(treeItem);
            }
        }

        private void SceneHierarchy_MouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                if (sender is StackPanel)
                {
                    var selectedItem = SceneTreeView.SelectedItem;
                    if (selectedItem != null)
                    {
                        ((OutlinerTreeItem)selectedItem).IsSelected = false;
                        ((EditorWindow)Application.Current.MainWindow).PropertiesPanel.LoadProperties(null);
                    }
                }
            }
        }
    }
}

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
            if (Project.IsLoaded)
            {
                ProjectItem.Visibility = Visibility.Visible;
                ProjectItem.ProjectName = Project.Name;
                ProjectItem.ContextMenu = new ProjectContextMenu();
                SceneHierarchy.ContextMenu = new ProjectContextMenu();
                foreach (Scene scene in SceneManager.Scenes)
                {
                    SceneTreeView.Items.Add(new OutlinerTreeItem(scene.Id, scene.Name, "/View/Images/ImageTest.png", ItemType.ITEM_TYPE_SCENE));
                    foreach (Entity entity in scene.Entities)
                    {
                        ((OutlinerTreeItem)SceneTreeView.Items[^1]).Items.Add(new OutlinerTreeItem(entity.Id, entity.Name, "./View/Images/MinimizeIcon.png", ItemType.ITEM_TYPE_ENTITY));
                    }
                }
            }
            
        }

        public void UpdateSceneGUI(Scene scene)
        {
            foreach (OutlinerTreeItem item in SceneTreeView.Items)
            {
                if (item.Id == scene.Id)
                {
                    item.Name = scene.Name;
                    item.Items.Clear();
                    foreach(Entity entity in scene.Entities)
                    {
                        item.Items.Add(new OutlinerTreeItem(entity.Id, entity.Name, "/View/Images/MinimizeIcon.png", ItemType.ITEM_TYPE_ENTITY));
                    }
                    item.IsExpanded = true;
                }
            }
        }

        public void UpdateGUI()
        {
            //TODO not recreate the whole tree here
            SceneTreeView.Items.Clear();
            foreach (Scene scene in SceneManager.Scenes)
            {
                int index = SceneTreeView.Items.Add(new OutlinerTreeItem(scene.Id, scene.Name, "/View/Images/ImageTest.png", ItemType.ITEM_TYPE_SCENE));
                foreach (Entity entity in scene.Entities)
                {
                    ((OutlinerTreeItem)SceneTreeView.Items[index]).Items.Add(new OutlinerTreeItem(entity.Id, entity.Name, "/View/Images/MinimizeIcon.png", ItemType.ITEM_TYPE_ENTITY));
                }
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

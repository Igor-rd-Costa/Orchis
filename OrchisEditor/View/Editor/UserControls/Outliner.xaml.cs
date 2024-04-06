using OrchisEditor.Controller.Editor;
using OrchisEditor.View.Editor.UserControls.OutlinerComponents;
using OrchisEditor.View.Editor.UserControls.OutlinerComponents.ContextMenus;
using System;
using System.Windows;
using System.Windows.Controls;

namespace OrchisEditor.View.Editor.UserControls
{
    public partial class Outliner : UserControl
    {
        public Outliner()
        {
            InitializeComponent();
            if (Project.IsLoaded)
            {
                ProjectItem.Visibility = Visibility.Visible;
                ProjectItem.ProjectName = Project.Name;
                ProjectItem.ContextMenu = new ProjectContextMenu();
                SceneHierarchy.ContextMenu = new ProjectContextMenu();
                for (int i = 0; i < Project.GetSceneCount(); i++) 
                {
                    Scene scene = Project.GetScene(i);
                    int index = SceneTreeView.Items.Add(new OutlinerTreeItem(scene.Name, "/View/Images/ImageTest.png", ItemType.ITEM_TYPE_SCENE));
                    for (int j = 0; j < scene.GetEntityCount(); j++) 
                    {
                        Entity entity = scene.GetEntity(j);
                        Console.WriteLine(SceneTreeView.Items[index].GetType());
                        ((OutlinerTreeItem)SceneTreeView.Items[index]).Items.Add(new OutlinerTreeItem(entity.Name, "/View/Images/MinimizeIcon.png", ItemType.ITEM_TYPE_ENTITY));
                    }
                }
            }
            
        }

        public void UpdateSceneGUI(Scene scene)
        {
            foreach (OutlinerTreeItem item in SceneTreeView.Items)
            {
                if (item.ItemName == scene.Name)
                {
                    item.Items.Clear();
                    for (int i = 0; i < scene.GetEntityCount();  i++)
                    {
                        Entity entity = scene.GetEntity(i);
                        item.Items.Add(new OutlinerTreeItem(entity.Name, "/View/Images/MinimizeIcon.png", ItemType.ITEM_TYPE_ENTITY));
                    }
                    item.IsExpanded = true;
                }
            }
        }

        public void UpdateGUI()
        {
            //TODO not recreate the whole tree here
            SceneTreeView.Items.Clear();
            for (int i = 0; i < Project.GetSceneCount(); i++)
            {
                Scene scene = Project.GetScene(i);
                int index = SceneTreeView.Items.Add(new OutlinerTreeItem(scene.Name, "/View/Images/ImageTest.png", ItemType.ITEM_TYPE_SCENE));
                for (int j = 0; j < scene.GetEntityCount(); j++)
                {
                    Entity entity = scene.GetEntity(j);
                    ((OutlinerTreeItem)SceneTreeView.Items[index]).Items.Add(new OutlinerTreeItem(entity.Name, "/View/Images/MinimizeIcon.png", ItemType.ITEM_TYPE_ENTITY));
                }
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
           
        }

        private void SceneTreeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (e.NewValue != null) 
            {
                OutlinerTreeItem treeItem = (OutlinerTreeItem)e.NewValue;
                ((EditorWindow)Application.Current.MainWindow).PropertiesPanel.LoadProperties(treeItem);
            }
        }
    }
}

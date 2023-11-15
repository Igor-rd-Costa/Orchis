using System.Windows.Controls;
using System.Windows.Media;
using System.Windows;
using System.Windows.Input;
using System;
using OrchisEditor.Controller.Editor;

namespace OrchisEditor.View.Editor.UserControls.OutlinerComponents.ContextMenus
{
    internal class SceneContextMenu : ContextMenu
    {
        public SceneContextMenu(OutlinerTreeItem item) 
        {
            Width = 130;
            DataContext = item;
            MenuItem addEntity = new() { Header = "Add entity" };
            addEntity.PreviewMouseLeftButtonDown += AddEntity_MouseLeftButtonDown;
            MenuItem deleteScene = new() { Header = "Delete" };
            deleteScene.PreviewMouseLeftButtonDown += DeleteScene_MouseLeftButtonDown;

            Items.Add(addEntity);
            Items.Add(deleteScene);
        }

        private void AddEntity_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Scene? scene = Project.GetScene(((OutlinerTreeItem)DataContext).ItemName);
            scene?.AddEntity();
        }

        private void DeleteScene_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (Project.IsLoaded)
                Project.RemoveScene(((OutlinerTreeItem)DataContext).ItemName);
        }

    }
}

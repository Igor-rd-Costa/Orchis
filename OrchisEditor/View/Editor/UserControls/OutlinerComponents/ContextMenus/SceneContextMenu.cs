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


            Items.Add(addEntity);
        }

        private void AddEntity_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            SceneManager.Scene?.AddEntity(Guid.Empty);
        }

    }
}

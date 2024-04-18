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
            MenuItem deleteScene = new() { Header = "Remove" };
            deleteScene.PreviewMouseLeftButtonDown += RemoveScene_MouseLeftButtonDown;
            MenuItem debugScene = new() { Header = "Debug" };
            debugScene.PreviewMouseLeftButtonDown += DebugScene_MouseLeftButtonDown;

            Items.Add(addEntity);
            Items.Add(debugScene);
            Items.Add(deleteScene);
        }

        private void AddEntity_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Scene? scene = SceneManager.GetScene(((OutlinerTreeItem)DataContext).Id);
            if (scene == null)
            {
                Console.WriteLine("Scene is null!");
                return;
            }
            scene.AddEntity(Guid.Empty);
        }

        private void RemoveScene_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            SceneManager.RemoveScene(((OutlinerTreeItem)DataContext).Id);
        }

        private void DebugScene_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Scene? scene = SceneManager.GetScene(((OutlinerTreeItem)DataContext).Id);
            if (scene == null)
            {
                Console.WriteLine("Scene is null!");
            }
            else
            {
                Console.WriteLine("Scene is not null!");
            }
            scene?.DebugEntities();
        }

    }
}

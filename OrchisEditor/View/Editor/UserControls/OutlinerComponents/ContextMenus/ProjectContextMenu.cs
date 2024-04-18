using OrchisEditor.Controller.Editor;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Input;

namespace OrchisEditor.View.Editor.UserControls.OutlinerComponents.ContextMenus
{
    internal class ProjectContextMenu : ContextMenu
    {

        public ProjectContextMenu() 
        {
            Width = 130;
            DataContext = this;
            MenuItem menuItem = new()
            {
                Header = "Add scene"
            };
            menuItem.PreviewMouseLeftButtonDown += AddScene_PreviewLeftMouseDown;

            MenuItem debugScenes = new() { Header =  "Debug" };
            debugScenes.PreviewMouseLeftButtonDown += DebugScenes_PreviewLeftMouseDown;

            Items.Add(menuItem);
            Items.Add(debugScenes);
        }

        private void AddScene_PreviewLeftMouseDown(object sender, MouseEventArgs e)
        {
            if (Project.IsLoaded)
                SceneManager.AddScene(Guid.Empty);
        }
        private void DebugScenes_PreviewLeftMouseDown(object sender, MouseEventArgs e)
        {
            SceneManager.DebugScenes();
        }
    }
}

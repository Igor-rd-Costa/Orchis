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
    internal class EntityContextMenu : ContextMenu
    {
        public EntityContextMenu(OutlinerTreeItem item)
        {
            Width = 130;
            DataContext = item;
            MenuItem menuItem = new()
            {
                Header = "Remove"
            };
            menuItem.PreviewMouseLeftButtonDown += Remove_MouseLeftButtonDown;
            Items.Add(menuItem);
        }

        private void Remove_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Scene? scene = Project.GetScene(((OutlinerTreeItem)((OutlinerTreeItem)DataContext).Parent).ItemName);
            scene?.RemoveEntity(((OutlinerTreeItem)DataContext).ItemName);
        }


    }
}

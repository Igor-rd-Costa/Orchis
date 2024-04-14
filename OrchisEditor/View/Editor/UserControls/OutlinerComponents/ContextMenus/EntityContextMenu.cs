using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Orchis;
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
    
            DataContext = item;
            MenuItem removeItem = new()
            {
                Header = "Remove"
            };
            removeItem.PreviewMouseLeftButtonDown += Remove_MouseLeftButtonDown;
            Items.Add(removeItem);

            MenuItem addComponentItem = new()
            {
                Header = "Add Component",
            };
            MenuItem transformComponent = new()
            {
                Header = "Tranform Component"
            };
            transformComponent.PreviewMouseLeftButtonDown += AddTransformComponent;
            addComponentItem.Items.Add(transformComponent);
            Items.Add(addComponentItem);
        }

        private void AddTransformComponent(object sender, MouseButtonEventArgs e)
        {
            Guid id = ((OutlinerTreeItem)DataContext).Id;
            Console.WriteLine("Parent is: " + ((OutlinerTreeItem)DataContext).Parent);
        }

        private void Remove_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            //Scene? scene = Project.GetScene(((OutlinerTreeItem)((OutlinerTreeItem)DataContext).Parent).ItemName);
            //scene?.RemoveEntity(((OutlinerTreeItem)DataContext).ItemName);
        }


    }
}

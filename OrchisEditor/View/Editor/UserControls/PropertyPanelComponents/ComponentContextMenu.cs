using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Editor.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Input;

namespace OrchisEditor.View.Editor.UserControls.PropertyPanelComponents
{
    internal class ComponentContextMenu : ContextMenu
    {
        private readonly Guid m_ComponentId;

        public ComponentContextMenu(Guid componentId)
        {
            m_ComponentId = componentId;
            MenuItem remove = new() { Header = "Remove" };
            remove.PreviewMouseLeftButtonDown += Remove_MouseLeftButtonDown;

            Items.Add(remove);
        }


        private void Remove_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            object? item = EditorWindow.GetPropertyPanel()?.SelectedItem;    
            if (item is Entity)
            {
                ((Entity)item).RemoveComponent(m_ComponentId);
                EditorWindow.GetPropertyPanel()?.UpdateGUI();
            }
        }
    }
}

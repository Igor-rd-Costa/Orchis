using OrchisEditor.Controller.Editor;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace OrchisEditor.View.Editor.UserControls.ToolsPanelComponents.AssetManagerComponents.ContextMenus
{
    internal class AssetManagerViewContextMenu : ContextMenu
    {
        public AssetManagerViewContextMenu() 
        {
            MenuItem newFolder = new() { Header = "New folder" };
            newFolder.Click += OnLeftMouseDown;
            Items.Add(newFolder);
        }

        private void OnLeftMouseDown(object sender, RoutedEventArgs e)
        {
            AssetManagerView? amv = EditorWindow.GetAssetManagerView();
            amv?.CreateFolder();
        }
    }
}

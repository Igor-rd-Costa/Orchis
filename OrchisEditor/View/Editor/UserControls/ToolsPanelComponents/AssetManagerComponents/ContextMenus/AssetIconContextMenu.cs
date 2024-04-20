using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Input;

namespace OrchisEditor.View.Editor.UserControls.ToolsPanelComponents.AssetManagerComponents.ContextMenus
{
    class AssetIconContextMenu : ContextMenu
    {
        public AssetIconContextMenu() 
        {
            MenuItem remove = new() { Header = "Remove" };
            remove.PreviewMouseLeftButtonDown += RemoveSelectedAssets;
            Items.Add(remove);
        }

        private void RemoveSelectedAssets(object sender, MouseButtonEventArgs e)
        {
            AssetManagerView? amv = EditorWindow.GetAssetManagerView();
            if (amv == null)
                return;

            amv.RemoveSelectedAssets();
        }

    }
}

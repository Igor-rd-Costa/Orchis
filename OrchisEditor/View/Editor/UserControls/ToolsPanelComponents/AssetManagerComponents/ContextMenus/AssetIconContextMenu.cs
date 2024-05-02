using System.Windows;
using System.Windows.Controls;

namespace OrchisEditor.View.Editor.UserControls.ToolsPanelComponents.AssetManagerComponents.ContextMenus
{
    class AssetIconContextMenu : ContextMenu
    {
        public AssetIconContextMenu() 
        {
            MenuItem remove = new() { Header = "Remove" };
            remove.Click += RemoveSelectedAssets;

            if (EditorWindow.GetAssetManagerView()?.SelectedCount <= 1)
            {
                MenuItem rename = new() { Header = "Rename" };
                rename.Click += RenameSelectedAsset;
                Items.Add(rename);
            }

            Items.Add(remove);
        }

        private void RemoveSelectedAssets(object sender, RoutedEventArgs e)
        {
            EditorWindow.GetAssetManagerView()?.RemoveSelectedAssets();
        }

        private void RenameSelectedAsset(object sender, RoutedEventArgs e)
        {
            EditorWindow.GetAssetManagerView()?.RenameSelectedAsset();
        }
    }
}

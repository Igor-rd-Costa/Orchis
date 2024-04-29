using Microsoft.VisualBasic;
using OrchisEditor.Controller.Editor;
using OrchisEditor.View.Editor.UserControls.ToolsPanelComponents.AssetManagerComponents;
using OrchisEditor.View.Editor.UserControls.ToolsPanelComponents.AssetManagerComponents.AssetImportForm;
using OrchisEditor.View.Editor.UserControls.ToolsPanelComponents.AssetManagerComponents.ContextMenus;
using OrchisEditor.View.UserControls;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace OrchisEditor.View.Editor.UserControls.ToolsPanelComponents
{
    /// <summary>
    /// Interaction logic for AssetManager.xaml
    /// </summary>
    public partial class AssetManagerView : UserControl
    {   
        private List<AssetIcon> m_SelectedItems = [];
        private string m_CurrentPath = "";

        public AssetManagerView()
        {
            InitializeComponent();
            DataContext = this;
            if (Project.IsLoaded)
            {
                AssetManager.OnChange(OnAssetChange);
                m_CurrentPath = AssetManager.Path;
                LoadAssets(m_CurrentPath);
            }
            Console.WriteLine("AssetManager init!");
        }

        public void RemoveSelectedAssets()
        {
            int folders = 0;
            int assets = 0;

            foreach(AssetIcon item in m_SelectedItems)
            {
                
                if (item.Type == AssetType.FOLDER)
                    folders++;
                else 
                    assets++;
            }    

            OrchisDialog dialog = new(OrchisDialogType.YES_NO);
            bool? result = dialog.ShowMessage($"Do you wish to delete {assets} assets and {folders} folders?");
            if (result == true)
            {
                List<AssetIcon> remove = []; 
                foreach (AssetIcon item in m_SelectedItems)
                {
                    if (item.Delete())
                    {
                        AssetView.Children.Remove(item);
                        remove.Add(item);
                    }
                }
                foreach (AssetIcon item in remove)
                    m_SelectedItems.Remove(item);
            }
        }

        private void OnAssetChange(AssetChangeEventArgs args)
        {
            int fileDot = args.Path.LastIndexOf('.');
            int fileBarIndex = args.Path.LastIndexOf('\\');
            bool hasFileInPath = fileDot != -1 && fileDot > fileBarIndex;
            
            if ((hasFileInPath && args.Path.Substring(0, fileBarIndex + 1) == m_CurrentPath) 
                || (!hasFileInPath && args.Path == m_CurrentPath))
            {
                Dispatcher.Invoke(() =>
                {
                    LoadAssets(m_CurrentPath);
                });
            }
        }

        private void WrapPanel_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (e.Source is not AssetIcon)
            {
                if (!Keyboard.IsKeyDown(Key.LeftCtrl))
                {
                    foreach (AssetIcon icon in m_SelectedItems)
                        icon.Selected = false;
                }
                return;
            }

            if (((AssetIcon)e.Source).Type == AssetType.FOLDER && e.ClickCount == 2)
            {
                ViewFolder(((AssetIcon)e.Source).Path);
                return;
            }


            if (!Keyboard.IsKeyDown(Key.LeftCtrl))
            {
                foreach (AssetIcon icon in m_SelectedItems)
                    icon.Selected = false;

                m_SelectedItems.Clear();
                ((AssetIcon)e.Source).Selected = true;
                m_SelectedItems.Add((AssetIcon)e.Source);
            }
            else
            {
                if (((AssetIcon)e.Source).Selected == true)
                {
                    ((AssetIcon)e.Source).Selected = false;
                    m_SelectedItems.Remove((AssetIcon)e.Source);
                }
                else
                {
                    ((AssetIcon)e.Source).Selected = true;
                    m_SelectedItems.Add((AssetIcon)e.Source);
                }
            }
        }

        private void WrapPanel_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (e.Source is not AssetIcon)
            {
                ContextMenu = null;
                foreach (AssetIcon icon in m_SelectedItems)
                    icon.Selected = false;
                m_SelectedItems.Clear();
                return;
            }
            ContextMenu = new AssetIconContextMenu();
            if (!Keyboard.IsKeyDown(Key.LeftCtrl))
            {
                foreach (AssetIcon icon in m_SelectedItems)
                    icon.Selected = false;
                m_SelectedItems.Clear();
                ((AssetIcon)e.Source).Selected = true;
                m_SelectedItems.Add((AssetIcon)e.Source);
            }
            else
            {
                if (!((AssetIcon)e.Source).Selected)
                {
                    ((AssetIcon)e.Source).Selected = true;
                    m_SelectedItems.Add((AssetIcon)e.Source);
                }
            }
        }

        private static bool ShouldIgnoreFile(string path)
        {
            return path.EndsWith(".oai") || path.EndsWith(".txt") || path.EndsWith(".osi");
        }

        private void LoadAssets(string path)
        {
            AssetView.Children.Clear();
            string[] assets = Directory.GetFileSystemEntries(path);
            foreach (var assetPath in assets)
            {
                if (ShouldIgnoreFile(assetPath))
                    continue;

                int separator = assetPath.LastIndexOf('\\') + 1;
                string name = assetPath.Substring(separator, assetPath.Length - separator);
                if (name.Contains('.'))
                {
                    AssetView.Children.Add(new AssetIcon(name, assetPath, AssetType.GENERIC));
                } 
                else
                {
                    AssetView.Children.Add(new AssetIcon(name, assetPath, AssetType.FOLDER));
                }
            }
        }

        private void ViewFolder(string path)
        {
            if (!Directory.Exists(path))
            {
                OrchisDialog dialog = new OrchisDialog(OrchisDialogType.OK);
                dialog.ShowMessage($"Unable to locate folder at {path}.");
                return;
            }

            m_CurrentPath = path;
            LoadAssets(m_CurrentPath);
            if (m_CurrentPath == AssetManager.Path)
                BackButton.IsEnabled = false;
            else
                BackButton.IsEnabled = true;
        }

        private void BackFolder()
        {
            int count = m_CurrentPath.LastIndexOf('\\') + 1;
            string newPath = m_CurrentPath.Substring(0, count);
            ViewFolder(newPath);
            //Should never happen but gonna check just in case.
            if (!m_CurrentPath.StartsWith(AssetManager.Path)) 
            {
                ViewFolder(m_CurrentPath);
            }
        }

        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            BackFolder();
        }
    }
}

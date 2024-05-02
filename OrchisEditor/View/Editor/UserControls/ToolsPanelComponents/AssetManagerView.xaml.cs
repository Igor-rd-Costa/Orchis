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
            EditorWindow.OnClick(OnGlobalClick);
            EditorWindow.OnKeyDown(OnGlobalKeyDown);
            if (Project.IsLoaded)
            {
                AssetManager.OnChange(OnAssetChange);
                m_CurrentPath = AssetManager.Path;
                LoadAssets(m_CurrentPath);
            }
        }
        public int SelectedCount { get { return m_SelectedItems.Count; } }

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
            Console.WriteLine($"Change: {args.Path}, {args.Change}");
            int fileDot = args.Path.LastIndexOf('.');
            int fileBarIndex = args.Path.LastIndexOf('\\');
            bool hasFileInPath = fileDot != -1 && fileDot > fileBarIndex;

            string path = args.Path.Substring(0, fileBarIndex);
            bool refreshView = (path == m_CurrentPath);

            if (refreshView)
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
                    m_SelectedItems.Clear();
                }
                return;
            }

            if (e.ClickCount == 2)
            {
                if (((AssetIcon)e.Source).Type == AssetType.FOLDER)
                {
                    ViewFolder(((AssetIcon)e.Source).Path);
                    foreach (AssetIcon icon in m_SelectedItems)
                        icon.Selected = false;
                    m_SelectedItems.Clear();
                    return;
                }
                if (((AssetIcon)e.Source).AssetName.EndsWith(".osn"))
                {
                    OrchisDialog dialog = new(OrchisDialogType.YES_NO);
                    bool? status = dialog.ShowMessage($"Load scene {((AssetIcon)e.Source).AssetName}?");
                    if (status.HasValue && status.Value)
                    {
                        SceneManager.LoadScene(((AssetIcon)e.Source).Path);
                        Project.RegisterChange();
                    }
                }
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
        private void OnGlobalClick(object sender, MouseButtonEventArgs e)
        {
            if (e.Source is ToolsPanel)
                return;
            foreach (AssetIcon icon in m_SelectedItems)
                icon.Selected = false;
        }

        private void OnGlobalKeyDown(object sender, KeyEventArgs e)
        {
            
            if (e.Key == Key.F2)
            {
                RenameSelectedAsset();
                e.Handled = true;
            }
        }

        private void WrapPanel_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            Console.WriteLine(e.Source);
            if (e.Source is AssetIcon)
            {
                if (!((AssetIcon)e.Source).Selected)
                {
                    foreach (AssetIcon item in m_SelectedItems)
                        item.Selected = false;
                    m_SelectedItems.Clear();
                    ((AssetIcon)e.Source).Selected = true;
                    m_SelectedItems.Add((AssetIcon)e.Source);
                }
                ContextMenu = new AssetIconContextMenu();
            }
            else
            {
                ContextMenu = new AssetManagerViewContextMenu();
                foreach (AssetIcon icon in m_SelectedItems)
                    icon.Selected = false;
                m_SelectedItems.Clear();
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
                string fullName = assetPath.Substring(separator, assetPath.Length - separator);
                if (fullName.Contains('.'))
                {
                    int point = fullName.IndexOf(".");
                    string name = fullName.Substring(0, point);
                    string ext = fullName.Substring(point);
                    AssetView.Children.Add(new AssetIcon(name, ext, assetPath, AssetType.GENERIC));
                } 
                else
                {
                    AssetView.Children.Add(new AssetIcon(fullName, "", assetPath, AssetType.FOLDER));
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
            UpdateNavBar();
        }

        private void BackFolder()
        {
            int count = m_CurrentPath.LastIndexOf('\\');
            string newPath = m_CurrentPath.Substring(0, count);
            ViewFolder(newPath);
            //Should never happen but gonna check just in case.
            if (!m_CurrentPath.StartsWith(AssetManager.Path)) 
            {
                ViewFolder(AssetManager.Path);
            }
        }

        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            BackFolder();
        }

        public void CreateFolder()
        {
            string dirName = "NewFolder";
            int count = 1;
            while (Directory.Exists($"{m_CurrentPath}\\{dirName}"))
            {
                dirName = $"NewFolder({count})";
                count++;
            }
            Directory.CreateDirectory($"{m_CurrentPath}\\{dirName}");
        }

        public void RenameSelectedAsset()
        {
            if (m_SelectedItems.Count != 1)
                return;

            m_SelectedItems[0].EnterRenameMode();
        }

        private void Home_Click(object sender, RoutedEventArgs e)
        {
            ViewFolder(AssetManager.Path);
        }

        private void UpdateNavBar()
        {
            NavBar.Children.Clear();
            string path = m_CurrentPath.Substring(AssetManager.Path.Length);
            if (path.Length == 0)
                return;

            string[] pathArray = path.Split('\\');

            foreach (string str in pathArray)
            {
                if (string.IsNullOrEmpty(str)) 
                    continue;

                TextBlock barBlock = new()
                {
                    Text = "\\",
                    Background = Brushes.Transparent,
                    Foreground = Brushes.DarkGray,
                    FontSize = 18,
                    Margin = new(2, -5, 0, 0)
                };
                TextBlock textBlock = new()
                {
                    Text = str,
                    Name= "PathSection",
                    Background = Brushes.Transparent,
                    Foreground = Brushes.DarkGray,
                    FontSize = 14,
                    Margin = new(2, -1, 0, 0)
                };
                textBlock.MouseEnter += (object sender, MouseEventArgs e) => 
                {
                    textBlock.Foreground = Brushes.LightGray;
                };
                textBlock.MouseLeave += (object sender, MouseEventArgs e) =>
                {
                    textBlock.Foreground = Brushes.DarkGray;
                };
                NavBar.Children.Add(barBlock);
                NavBar.Children.Add(textBlock);
            }
        }

        private void NavBar_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Console.WriteLine("Click");

            if (e.Source is TextBlock textBlock && textBlock.Name == "PathSection")
            {
                string path = AssetManager.Path;
                foreach (UIElement child in NavBar.Children)
                {
                    if (child is TextBlock && ((TextBlock)child).Name == "PathSection")
                        path += $"\\{((TextBlock)child).Text}";

                    if (child == e.Source)
                        break;
                }
                if (path == m_CurrentPath)
                    return;
                ViewFolder(path);
            }
        }
    }
}

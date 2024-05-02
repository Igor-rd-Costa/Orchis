using OrchisEditor.Controller.Editor;
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
using static System.Net.Mime.MediaTypeNames;

namespace OrchisEditor.View.Editor.UserControls.ToolsPanelComponents.AssetManagerComponents
{
    /// <summary>
    /// Interaction logic for AssetIcon.xaml
    /// </summary>
    
    public struct AssetIconDragData
    {
        public string Name;
        public string Ext;
        public string Path;
    }

    public partial class AssetIcon : UserControl
    {
        private AssetType m_Type = AssetType.GENERIC;
        private string m_Name = "";
        private string m_Ext = "";
        private string m_Path = "";
        private bool m_Selected = false;
        private bool m_InRenameMode = false;
        public AssetIcon()
        {
            InitializeComponent();
            DataContext = this;
            ToolTip = AssetName;
            Icon.Background = GetIcon(m_Type);
            if (m_Type == AssetType.FOLDER)
            {
                SelectionGrid.AllowDrop = true;
                Drop += OnDrop;
            }
        }

        public AssetIcon(string name, string ext, string path, AssetType type)
        {
            InitializeComponent();
            DataContext = this;
            m_Name = name;
            m_Ext = ext;
            m_Path = path;
            m_Type = type;
            ToolTip = AssetName;
            Icon.Background = GetIcon(m_Type);
            if (m_Type == AssetType.FOLDER)
            {
                SelectionGrid.AllowDrop = true;
                Drop += OnDrop;
            }
        }

        public bool Selected
        {
            get { return m_Selected; } 
            set 
            { 
                m_Selected = value;
                if (value)
                {
                    SelectionGrid.Background = (Brush)new BrushConverter().ConvertFrom("#33E75CA4")!;
                    SelectionBorder.BorderBrush = (Brush)new BrushConverter().ConvertFrom("#FFE75CA4")!;
                }
                else
                {
                    SelectionGrid.Background = Brushes.Transparent;
                    SelectionBorder.BorderBrush = Brushes.Transparent;
                    LeaveRenameMode();
                }
            }
        }
        public AssetType Type { get { return m_Type; } }
        public string AssetName 
        { 
            get { return m_Name+m_Ext; } 
        }
        public string Path
        {
            get { return m_Path; }
        }
        public bool Delete()
        {
            try
            {
                if (Type == AssetType.FOLDER)
                {
                    Directory.Delete(m_Path, true);
                }
                else
                {
                    if (m_Path.EndsWith(".osn"))
                    {
                        if (SceneManager.Scene != null && m_Path.EndsWith($"{SceneManager.Scene.Name}.osn"))
                        {
                            OrchisDialog dialog = new(OrchisDialogType.OK);
                            dialog.ShowMessage($"Could not delete scene file {AssetName} because it is currently in use.");
                            return false;
                        }

                        AssetManager.DeleteSceneInfo(m_Path);
                    }

                    File.Delete(m_Path);
                }
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        public void EnterRenameMode()
        {
            if (m_InRenameMode) 
                return;

            AssetNameField.Visibility = Visibility.Hidden;
            AssetRenameField.Visibility = Visibility.Visible;
            AssetRenameField.Text = m_Name;
            m_InRenameMode = true;
            AssetRenameField.Focus();
        }

        public void LeaveRenameMode()
        {
            if (!m_InRenameMode)
                return;

            if (AssetRenameField.Text != m_Name)
            {
                string newNameBase = AssetRenameField.Text;
                Console.WriteLine($"Renaming {m_Name} to {newNameBase}!");
                Console.WriteLine($"Path: {m_Path}");
                string path = m_Path[..(m_Path.LastIndexOf('\\') + 1)];
                string newName = newNameBase;
                uint count = 1;
                if (m_Type == AssetType.FOLDER)
                {
                    while (Directory.Exists(path+newName)) 
                    {
                        newName = $"{newNameBase}({count})";
                        count++;
                    }
                    Directory.Move(m_Path, path+newName);
                }
                else
                {
                    while (File.Exists(path+newName))
                    {
                        newName = $"{newNameBase}({count})";
                        count++;
                    }
                    File.Move(m_Path, path+newName+m_Ext);
                    AssetManager.UpdateAssetPath(m_Path, path + newName + m_Ext);
                }
                m_Path = path + newName;
                m_Name = newName;
            }

            AssetNameField.Visibility = Visibility.Visible;
            AssetRenameField.Visibility = Visibility.Hidden;
            AssetNameField.Text = m_Name + m_Ext;
            m_InRenameMode = false;
        }

        //TODO make this generate an icon
        private static SolidColorBrush GetIcon(AssetType type)
        {
            switch (type)
            {
                case AssetType.FOLDER: return Brushes.Purple;
                default: return Brushes.RosyBrown;
            }
        }

        private void SelectionGrid_MouseLeave(object sender, MouseEventArgs e)
        {
            if (Mouse.LeftButton == MouseButtonState.Pressed)
            {
                AssetIconDragData data = new() { Name = m_Name, Ext = m_Ext, Path = m_Path };
                DragDrop.DoDragDrop(this, data, DragDropEffects.Copy);
            }
        }

        private void AssetRenameField_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            foreach(char c in e.Text)
            {
                if (!char.IsLetterOrDigit(c) && c != ' ')
                {
                    e.Handled = true;
                    return;
                }
            }
        }

        private void OnDrop(object sender, DragEventArgs e)
        {
            string format = typeof(AssetIconDragData).ToString();
            if (e.Data.GetDataPresent(format))
            {
                AssetIconDragData data = (AssetIconDragData)e.Data.GetData(format);
                File.Move(data.Path, $"{m_Path}\\{data.Name}{data.Ext}");
                AssetManager.UpdateAssetPath(data.Path, $"{m_Path}\\{data.Name}{data.Ext}");
            }
        }

        private void AssetRenameField_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter) 
            {
                LeaveRenameMode();
            }
        }
    }
}

using OrchisEditor.Controller.Editor;
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

namespace OrchisEditor.View.Editor.UserControls.ToolsPanelComponents.AssetManagerComponents
{
    /// <summary>
    /// Interaction logic for AssetIcon.xaml
    /// </summary>
    
    public struct AssetIconDragData
    {
        public string Name;
        public string Path;
    }

    public partial class AssetIcon : UserControl
    {
        private AssetType m_Type = AssetType.GENERIC;
        private string m_Name = "";
        private string m_Path = "";
        private bool m_Selected = false;

        public AssetIcon()
        {
            InitializeComponent();
            DataContext = this;
            Icon.Background = GetIcon(m_Type);
            if (m_Type != AssetType.FOLDER)
                SelectionGrid.AllowDrop = true;
        }

        public AssetIcon(string name, string path, AssetType type)
        {
            InitializeComponent();
            DataContext = this;
            m_Name = name;
            m_Path = path;
            m_Type = type;
            Icon.Background = GetIcon(m_Type);
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
                }
            }
        }
        public AssetType Type { get { return m_Type; } }
        public string AssetName 
        { 
            get { return m_Name; } 
            set { m_Name = value; } 
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
                    File.Delete(m_Path);
                }
                return true;
            }
            catch (Exception)
            {
                return false;
            }
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
            AssetIconDragData data = new() { Name = m_Name, Path = m_Path };
            DragDrop.DoDragDrop(this, data, DragDropEffects.Copy);
        }
    }
}

using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Editor.Components;
using OrchisEditor.Controller.Orchis;
using OrchisEditor.View.Editor.UserControls.ToolsPanelComponents.AssetManagerComponents;
using System;
using System.Collections.Generic;
using System.ComponentModel;
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

namespace OrchisEditor.View.Editor.UserControls.PropertyPanelComponents
{
    /// <summary>
    /// Interaction logic for TextureComponentItem.xaml
    /// </summary>
    public partial class TextureComponentItem : UserControl
    {
        private Guid m_Id;
        private Guid m_TextureId = Guid.Empty;
        private string m_TextureName = "";
        private string m_TexturePath = "";
        public TextureComponentItem(Guid componentId, Guid textureId)
        {
            DataContext = this;
            InitializeComponent();
            m_Id = componentId;
            m_TextureId = textureId;
            if (textureId == Guid.Empty)
                m_TextureName = "Empty";
            else
            {
                AssetInfo? info = AssetManager.GetAssetInfo(textureId);
                if (info != null)
                {
                    m_TextureName = info.Value.Name;
                    m_TexturePath = info.Value.Path;
                    TextureIcon.ToolTip = m_TexturePath;
                }
            }
            TextureNameField.Text = m_TextureName;
        }

        private void OnDrop(object sender, DragEventArgs e)
        {
            string format = typeof(AssetIconDragData).ToString();
            if (e.Data.GetDataPresent(format))
            {
                AssetIconDragData data = (AssetIconDragData)e.Data.GetData(format);
                m_TextureName = data.Name;
                m_TexturePath = data.Path;

                TextureNameField.Text = data.Name;
                TextureIcon.ToolTip = m_TexturePath;
                TextureNameField.ToolTip = m_TexturePath;

                TextureNameField.Foreground = Brushes.LightGray;
                Guid id = AssetManager.RegisterAsset(data.Path);
                if (m_TextureId != id)
                {
                    m_TextureId = id;
                    OrchisInterface.OrchisComponentManagerUpdateTextureComponentTextureId(m_Id, m_TextureId);
                    SceneManager.RegisterChange();
                }
            }
            SetBorderColor(Brushes.Gray);
        }

        private void OnDragEnter(object sender, DragEventArgs e)
        {
            SetBorderColor((SolidColorBrush)FindResource("MainBrush"));
        }
        private void OnDragLeave(object sender, DragEventArgs e)
        {
            SetBorderColor(Brushes.Gray);
        }

        private void SetBorderColor(SolidColorBrush brush)
        {
            ((Border)TextureIcon.Children[0]).BorderBrush = brush;
            ((Border)TextureIcon.Children[1]).BorderBrush = brush;
            ((Border)TextureIcon.Children[2]).BorderBrush = brush;
            ((Border)TextureIcon.Children[2]).Background = brush;
            TextureNameFieldBorder.BorderBrush = brush;
        }
    }
}

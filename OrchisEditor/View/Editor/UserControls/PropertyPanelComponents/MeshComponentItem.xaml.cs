using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Editor.Components;
using OrchisEditor.Controller.Orchis;
using OrchisEditor.View.Editor.UserControls.ToolsPanelComponents.AssetManagerComponents;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;


namespace OrchisEditor.View.Editor.UserControls.PropertyPanelComponents
{
    /// <summary>
    /// Interaction logic for MeshComponentItem.xaml
    /// </summary>
    public partial class MeshComponentItem : UserControl
    {
        private Guid m_Id;
        private Guid m_AssetId = Guid.Empty;
        private MeshType m_Type = MeshType.STATIC;
        private string m_MeshName = "";
        private string m_MeshPath = "";

        public MeshComponentItem(Guid componentId, Guid meshId, MeshType meshType)
        {
            DataContext = this;
            InitializeComponent();
            m_Id = componentId;
            m_AssetId = meshId;
            m_Type = meshType;
            if (meshId == Guid.Empty)
                MeshNameField.Text = "Empty";
            else
            {
                AssetInfo? info = AssetManager.GetAssetInfo(meshId);
                if (info != null)
                {
                    m_MeshName = info.Value.Name;
                    m_MeshPath = info.Value.Path;
                    MeshIcon.ToolTip = m_MeshPath;
                    MeshNameField.ToolTip = m_MeshPath;
                }
            }
            MeshNameField.Text = m_MeshName;
        }

        private void OnDrop(object sender, DragEventArgs e)
        {
            string format = typeof(AssetIconDragData).ToString();
            if (e.Data.GetDataPresent(format))
            {
                AssetIconDragData data = (AssetIconDragData)e.Data.GetData(format);
                m_MeshName = data.Name;
                m_MeshPath = data.Path;

                MeshNameField.Text = data.Name;
                MeshIcon.ToolTip = m_MeshPath;
                MeshNameField.ToolTip = m_MeshPath;

                MeshNameField.Foreground = Brushes.LightGray;
                Guid id = AssetManager.RegisterAsset(data.Path);
                if (m_AssetId != id)
                {
                    m_AssetId = id;
                    OrchisInterface.OrchisComponentManagerUpdateMeshComponentMeshId(m_Id, m_AssetId);
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
            ((Border)MeshIcon.Children[0]).BorderBrush = brush;
            ((Border)MeshIcon.Children[1]).BorderBrush = brush;
            ((Border)MeshIcon.Children[2]).BorderBrush = brush;
            ((Border)MeshIcon.Children[2]).Background = brush;
            MeshNameFieldBorder.BorderBrush = brush;
        }
    }
}

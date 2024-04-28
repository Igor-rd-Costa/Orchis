using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Orchis;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
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
    /// Interaction logic for TransformComponent.xaml
    /// </summary>
    public partial class TransformComponentItem : UserControl
    {
        private Guid m_Id;
        public TransformComponentItem(Guid componentId)
        {
            InitializeComponent();
            DataContext = this;
            m_Id = componentId;
            PosX.Value = 0.0M;
            PosY.Value = 0.0M;
            PosZ.Value = 0.0M;

            RotX.Value = 0.0M;
            RotY.Value = 0.0M;
            RotZ.Value = 0.0M;

            SclX.Value = 0.0M;
            SclY.Value = 0.0M;
            SclZ.Value = 0.0M;
        }

        public TransformComponentItem(Guid componentId, Vector3 pos, Vector3 rot, Vector3 scl)
        {
            InitializeComponent();
            DataContext = this;
            m_Id = componentId;
            PosX.Value = (decimal)pos.X;
            PosY.Value = (decimal)pos.Y;
            PosZ.Value = (decimal)pos.Z;

            RotX.Value = (decimal)rot.X;
            RotY.Value = (decimal)rot.Y;
            RotZ.Value = (decimal)rot.Z;

            SclX.Value = (decimal)scl.X;
            SclY.Value = (decimal)scl.Y;
            SclZ.Value = (decimal)scl.Z;
        }

        private void OnInputChange(object sender, decimal val)
        {
            if (sender == PosX || sender == PosY || sender == PosZ)
            {
                Engine.ComponentManager.TransformComponent.SetPosition(m_Id, new((float)PosX.Value, (float)PosY.Value, (float)PosZ.Value));
            }
            if (sender == RotX || sender == RotY || sender == RotZ)
            {
                Engine.ComponentManager.TransformComponent.SetRotation(m_Id, new((float)RotX.Value, (float)RotY.Value, (float)RotZ.Value));
            }
            if (sender == SclX || sender == SclY || sender == SclZ)
            {
                Engine.ComponentManager.TransformComponent.SetScale(m_Id, new((float)SclX.Value, (float)SclY.Value, (float)SclZ.Value));
            }

            if (!((App)Application.Current).IsScenePlaying)
                SceneManager.RegisterChange();
        }
    }
}

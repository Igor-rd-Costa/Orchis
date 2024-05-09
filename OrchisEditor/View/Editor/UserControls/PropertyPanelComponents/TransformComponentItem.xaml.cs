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
            PosX.Value = 0.0F;
            PosY.Value = 0.0F;
            PosZ.Value = 0.0F;

            RotX.Value = 0.0F;
            RotY.Value = 0.0F;
            RotZ.Value = 0.0F;

            SclX.Value = 0.0F;
            SclY.Value = 0.0F;
            SclZ.Value = 0.0F;
        }

        public TransformComponentItem(Guid componentId, Vector3 pos, Vector3 rot, Vector3 scl)
        {
            InitializeComponent();
            DataContext = this;
            m_Id = componentId;
            PosX.Value = pos.X;
            PosY.Value = pos.Y;
            PosZ.Value = pos.Z;

            RotX.Value = rot.X;
            RotY.Value = rot.Y;
            RotZ.Value = rot.Z;

            SclX.Value = scl.X;
            SclY.Value = scl.Y;
            SclZ.Value = scl.Z;
        }

        private void OnInputChange(object sender, float val)
        {
            if (sender == PosX || sender == PosY || sender == PosZ)
            {
                Engine.ComponentManager.TransformComponent.SetPosition(m_Id, new(PosX.Value, PosY.Value, PosZ.Value));
            }
            if (sender == RotX || sender == RotY || sender == RotZ)
            {
                Engine.ComponentManager.TransformComponent.SetRotation(m_Id, new(RotX.Value, RotY.Value, RotZ.Value));
            }
            if (sender == SclX || sender == SclY || sender == SclZ)
            {
                Engine.ComponentManager.TransformComponent.SetScale(m_Id, new(SclX.Value, SclY.Value, SclZ.Value));
            }

            if (!((App)Application.Current).IsScenePlaying)
                SceneManager.RegisterChange();
        }
    }
}

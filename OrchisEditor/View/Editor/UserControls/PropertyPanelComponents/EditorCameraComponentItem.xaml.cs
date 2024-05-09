using OrchisEditor.Controller.Orchis;
using System.Windows;
using System.Windows.Controls;


namespace OrchisEditor.View.Editor.UserControls.PropertyPanelComponents
{
    /// <summary>
    /// Interaction logic for CameraComponentItem.xaml
    /// </summary>
    public partial class EditorCameraComponentItem : UserControl
    {
        private OrchisInterface.EditorCameraMoveEventCallback m_CameraMoveCallback;
        public EditorCameraComponentItem()
        {
            InitializeComponent();
            UpdateGUI();
            m_CameraMoveCallback = OnCameraMove;
            Engine.Editor.RegisterCameraMoveEventCallback(m_CameraMoveCallback);
        }

        private void OnInputChange(object sender, float val)
        {
            EditorCameraData data = Engine.Editor.GetCameraData();
            string name = ((FrameworkElement)sender).Name;
            if (sender == PosX || sender == PosY || sender == PosZ)
            {
                data.Position = new(PosX.Value, PosY.Value, PosZ.Value);
            }
            if (sender == LookAtX || sender == LookAtY || sender == LookAtZ)
            {
                data.LookAt = new(LookAtX.Value, LookAtY.Value, LookAtZ.Value);
            }
            if (sender == Speed)
            {
                data.Speed = Speed.Value;
            }
            if (sender == FoV)
            {
                data.FieldOfView = FoV.Value;
            }
            Engine.Editor.SetCameraData(data);
        }

        private void OnCameraMove(EditorCameraMoveEventArgs e)
        {
            Dispatcher.Invoke(() =>
            {
                PosX.Value = e.Data.Position.X;
                PosY.Value = e.Data.Position.Y;
                PosZ.Value = e.Data.Position.Z;
                LookAtX.Value = e.Data.LookAt.X;
                LookAtY.Value = e.Data.LookAt.Y;
                LookAtZ.Value = e.Data.LookAt.Z;
                Speed.Value = e.Data.Speed;
                FoV.Value = e.Data.FieldOfView;
            });
        }

        private void UserControl_Unloaded(object sender, RoutedEventArgs e)
        {
            Engine.Editor.RegisterCameraMoveEventCallback((EditorCameraMoveEventArgs e) => { });
        }

        private void UpdateGUI()
        {
            EditorCameraData data = Engine.Editor.GetCameraData();
            PosX.Value = data.Position.X;
            PosY.Value = data.Position.Y;
            PosZ.Value = data.Position.Z;
            LookAtX.Value = data.LookAt.X;
            LookAtY.Value = data.LookAt.Y;
            LookAtZ.Value = data.LookAt.Z;
            Speed.Value = data.Speed;
            FoV.Value = data.FieldOfView;
        }
    }
}

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
        private bool m_Visible = false;
        public TransformComponentItem(Guid componentId)
        {
            InitializeComponent();
            UpdateVisibility();
            Heading.ContextMenu = new ComponentContextMenu(componentId);
            PosX.Text = "0.0";
            PosY.Text = "0.0";
            PosZ.Text = "0.0";

            RotX.Text = "0.0";
            RotY.Text = "0.0";
            RotZ.Text = "0.0";

            SclX.Text = "0.0";
            SclY.Text = "0.0";
            SclZ.Text = "0.0";
        }

        public TransformComponentItem(Guid componentId, Vector3 pos, Vector3 rot, Vector3 scl, bool expanded = false)
        {
            Heading.ContextMenu = new ComponentContextMenu(componentId);
            InitializeComponent();
            m_Visible = expanded;
            UpdateVisibility();
            PosX.Text = pos.X.ToString();
            PosY.Text = pos.Y.ToString();
            PosZ.Text = pos.Z.ToString();

            RotX.Text = rot.X.ToString();
            RotY.Text = rot.Y.ToString();
            RotZ.Text = rot.Z.ToString();

            SclX.Text = scl.X.ToString();
            SclY.Text = scl.Y.ToString();
            SclZ.Text = scl.Z.ToString();
        }

        private void OnRetractButtonClick(object sender, RoutedEventArgs e)
        {
            m_Visible = !m_Visible;
            UpdateVisibility();
        }

        private void OnKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Space)
            {
                e.Handled = true;
                return;
            }
        }

        private void OnTextPreview(object sender, TextCompositionEventArgs e)
        {
            Console.WriteLine(e.Text);
            foreach (char c in e.Text)
            {
                if (c == ',' && !((TextBox)sender).Text.Contains(','))
                    continue;
                if (!char.IsDigit(c))
                {
                    e.Handled = true;
                    return;
                }
            }
        }

        private void UpdateVisibility()
        {
            if (m_Visible)
            {
                Transforms.Visibility = Visibility.Visible;
                BottomBorder.Visibility = Visibility.Visible;
            }
            else
            {
                Transforms.Visibility = Visibility.Hidden;
                BottomBorder.Visibility = Visibility.Hidden;
            }
        }

        private void HeaderOnLeftMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount == 2)
            {
                m_Visible = !m_Visible;
                UpdateVisibility();
            }
        }

        private void HeaderOnRightMouseDown(object sender, MouseButtonEventArgs e)
        {

        }
    }
}

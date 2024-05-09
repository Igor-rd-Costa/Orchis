using OrchisEditor.View.Editor;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace OrchisEditor.View.UserControls
{
    /// <summary>
    /// Interaction logic for RangedNumberInput.xaml
    /// </summary>
    public partial class RangedNumberInput : UserControl, INotifyPropertyChanged
    {
        private static double s_LastMousePos = 0.0;
        private bool m_IsMouseDown = false;
        private float m_RangeMin = 0;
        private float m_RangeMax = 10;
        private float m_Value = 0;
        private float m_RangeStep = 0;

        public event PropertyChangedEventHandler? PropertyChanged;
        public event EventHandler<float>? Change;

        public float MinRange
        {
            get { return m_RangeMin; }
            set 
            { 
                m_RangeMin = value;
                if (m_Value < m_RangeMin)
                    Value = m_RangeMin;
            }
        }
        public float MaxRange
        {
            get { return m_RangeMax; }
            set 
            { 
                m_RangeMax = value;
                if (m_Value > m_RangeMax)
                    Value = m_RangeMax;
            }
        }
        public float Value
        {
            get { return m_Value; }
            set 
            {
                m_Value = value;
                if (m_Value > m_RangeMax)
                    m_Value = m_RangeMax;
                if (m_Value < m_RangeMin)
                    m_Value = m_RangeMin;
                Selector.Margin = new(((m_Value - m_RangeMin) * m_RangeStep), 0, 0, 0);
                PropertyChanged?.Invoke(this, new(nameof(Value)));
            }
        }

        public void SetValue(float value)
        {
            m_Value = value;
            if (m_Value > m_RangeMax)
                m_Value = m_RangeMax;
            if (m_Value < m_RangeMin)
                m_Value = m_RangeMin;
            Selector.Margin = new(((m_Value - m_RangeMin) * m_RangeStep), 0, 0, 0);
            PropertyChanged?.Invoke(this, new(nameof(Value)));
            Change?.Invoke(this, m_Value);
        }
        public RangedNumberInput()
        {
            InitializeComponent();
            DataContext = this;
            EditorWindow.OnMouseLeftButtonUp(Selector_MouseLeftButtonUp);
            EditorWindow.OnMouseMove(Selector_MouseMove);
        }

        private void Selector_MouseEnter(object sender, MouseEventArgs e)
        {
            Selector.Background = (SolidColorBrush)new BrushConverter().ConvertFrom("#808080");
        }
        private void Selector_MouseLeave(object sender, MouseEventArgs e)
        {
            if (!m_IsMouseDown)
                Selector.Background = (SolidColorBrush)new BrushConverter().ConvertFrom("#606060");
        }
        private void Selector_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            s_LastMousePos = e.GetPosition(Selector).X;
            m_IsMouseDown = true;
            NumberDisplayWrapper.Visibility = Visibility.Visible;
            double displayLeft = (-(NumberDisplayWrapper.ActualWidth / 2)) + (Selector.Width / 2) + Selector.Margin.Left;
            if (displayLeft + NumberDisplayWrapper.ActualWidth > Wrapper.ActualWidth)
                displayLeft = Wrapper.ActualWidth - NumberDisplayWrapper.ActualWidth;
            NumberDisplayWrapper.Margin = new(displayLeft, -35, 0, 0);
        }
        private void Selector_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (m_IsMouseDown)
            {
                Point pos = e.GetPosition(Selector);
                if (pos.X < 0 || pos.X > Selector.Width
                    || pos.Y < 0 || pos.Y > Selector.Height)
                {
                    Selector.Background = (SolidColorBrush)new BrushConverter().ConvertFrom("#606060");
                }
                m_IsMouseDown = false;
                NumberDisplayWrapper.Visibility = Visibility.Hidden;
            }
        }
        private void Selector_MouseMove(object sender, MouseEventArgs e)
        {
            if (!m_IsMouseDown)
                return;

            double offset = e.GetPosition(Selector).X - s_LastMousePos;
            double newVal = Selector.Margin.Left + offset;
            if (newVal < 0) 
                newVal = 0;

            if (newVal > RangeLine.ActualWidth - Selector.Width)
                newVal = RangeLine.ActualWidth - Selector.Width;

            Selector.Margin = new(newVal, 0, 0, 0);
            double displayLeft = (-(NumberDisplayWrapper.ActualWidth / 2)) + (Selector.Width / 2) + newVal;
            if (displayLeft + NumberDisplayWrapper.ActualWidth > Wrapper.ActualWidth)
                displayLeft = Wrapper.ActualWidth - NumberDisplayWrapper.ActualWidth;
            NumberDisplayWrapper.Margin = new(displayLeft, -35, 0, 0);

            string valStr = ((float)newVal / m_RangeStep).ToString();
            int point = valStr.IndexOf('.');
            if (point != -1)
                valStr = valStr.Substring(0, point + 3);
            float val = float.Parse(valStr);
            if (val != m_Value)
            {
                SetValue(val + m_RangeMin);
            }
        }

        private void Wrapper_Loaded(object sender, RoutedEventArgs e)
        {
            m_RangeStep = (float)(RangeLine.ActualWidth - Selector.Width) / (m_RangeMax - m_RangeMin);
            Selector.Margin = new(((m_Value - m_RangeMin)* m_RangeStep), 0, 0, 0);
        }
    }
}

using System;
using System.Collections.Generic;
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

namespace OrchisEditor.View.UserControls
{
    /// <summary>
    /// Interaction logic for NumberInput.xaml
    /// </summary>
    public partial class NumberInput : UserControl
    {
        private float m_Value = 0.0F;
        public event EventHandler<float>? Change;

        public NumberInput()
        {
            InitializeComponent();
            DataContext = this;
            DataObject.AddPastingHandler(Input, OnPaste);
            Loaded += (object sender, RoutedEventArgs e) => {
                Input.TextChanged += OnTextChanged;
            };
        }
        public float Value
        {
            get { return m_Value; }
            set 
            { 
                m_Value = value;
                Input.Text = m_Value.ToString();
            }
        }
        public void SetValue(float value)
        {
            m_Value = value;
            Input.Text = m_Value.ToString();
            Change?.Invoke(this, value);
        }

        private void OnKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Space)
            {
                e.Handled = true;
            }
            if (e.Key == Key.Up || e.Key == Key.Down)
            {
                if (e.Key == Key.Up)
                {
                    if (Keyboard.IsKeyDown(Key.LeftCtrl))
                        SetValue(Value + 0.1F);
                    else if (Keyboard.IsKeyDown(Key.LeftShift))
                        SetValue(Value + 10.0F);
                    else
                        SetValue(Value + 1.0F);
                }
                else
                {
                    if (Keyboard.IsKeyDown(Key.LeftCtrl))
                        SetValue(Value - 0.1F);
                    else if (Keyboard.IsKeyDown(Key.LeftShift))
                        SetValue(Value - 10.0F);
                    else
                        SetValue(Value - 1.0F);
                }
                e.Handled = true;
            }
        }
        private void OnTextInputPreview(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !IsTextValid(e.Text);
        }
        private void OnPaste(object sender, DataObjectPastingEventArgs e)
        {
            e.CancelCommand();
        }
        private void OnTextChanged(object sender, TextChangedEventArgs e)
        {
            string text = Input.Text;
            float val = float.Parse(text == String.Empty ? "0" : text);
            m_Value = val;
        }
        private bool IsTextValid(string text)
        {
            foreach (char c in text)
            {
                if (c == '.' && !Input.Text.Contains('.'))
                    return true;
                if (!char.IsDigit(c))
                    return false;

                int point = Input.Text.IndexOf('.');
                if (point != -1 && Input.Text.Length - point > 2)
                    return false;
            }
            return true;
        }
    }
}

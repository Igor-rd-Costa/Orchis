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
        private decimal m_Value = 0.0M;
        public event EventHandler<decimal>? Change;

        public NumberInput()
        {
            InitializeComponent();
            DataContext = this;
            DataObject.AddPastingHandler(Input, OnPaste);
            Loaded += (object sender, RoutedEventArgs e) => {
                Input.TextChanged += OnTextChanged;
            };
        }
        public decimal Value
        {
            get { return m_Value; }
            set 
            { 
                m_Value = value;
                Input.Text = m_Value.ToString();
            }
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
                        Value += 0.1M;
                    else if (Keyboard.IsKeyDown(Key.LeftShift))
                        Value += 10.0M;
                    else
                        Value += 1.0M;
                }
                else
                {
                    if (Keyboard.IsKeyDown(Key.LeftCtrl))
                        Value -= 0.1M;
                    else if (Keyboard.IsKeyDown(Key.LeftShift))
                        Value -= 10.0M;
                    else
                        Value -= 1.0M;
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
            decimal val = Convert.ToDecimal(text == String.Empty ? "0" : text);
            m_Value = val;
            Change?.Invoke(this, val);
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

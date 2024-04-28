using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace OrchisEditor.View.Editor.UserControls.PropertyPanelComponents.UserControls
{
    partial class ComponentItemBase : ContentControl
    {
        private bool m_IsVisible = true;
        public static readonly DependencyProperty m_HeaderTextProperty =
            DependencyProperty.Register("HeaderText", typeof(string), typeof(ComponentItemBase), new PropertyMetadata(""));

        private Grid ContentDisplay
        {
            get { return (Grid)Template.FindName("ContentDisplay", this); }
        }
        private Border BottomBorder
        {
            get { return (Border)Template.FindName("BottomBorder", this); }
        }
        private Border TopBorder
        {
            get { return (Border)Template.FindName("TopBorder", this); }
        }
        private Grid Heading
        {
            get { return (Grid)Template.FindName("Heading", this); }
        }
        private Button RetractButton
        {
            get { return (Button)Template.FindName("RetractButton", this); }
        }
        public ComponentItemBase() 
        {
            DataContext = this;
            Loaded += ComponentItemBase_Loaded;
        }

        private void ComponentItemBase_Loaded(object sender, RoutedEventArgs e)
        {
            TopBorder.Visibility = m_IsVisible ? Visibility.Collapsed : Visibility.Visible;
            BottomBorder.Visibility = m_IsVisible ? Visibility.Visible : Visibility.Collapsed; 
            Heading.MouseLeftButtonDown += Heading_MouseLeftButtonDown;
            RetractButton.Click += Retract_Click;
        }

        public string HeaderText
        {
            get { return (string)GetValue(m_HeaderTextProperty); }
            set { SetValue(m_HeaderTextProperty, value); }
        }

        private void UpdateVisibility()
        {
            if (m_IsVisible)
            {
                ContentDisplay.Visibility = Visibility.Visible;
                BottomBorder.Visibility = Visibility.Visible;
                TopBorder.Visibility = Visibility.Collapsed;
            }
            else
            {
                ContentDisplay.Visibility = Visibility.Collapsed;
                BottomBorder.Visibility = Visibility.Collapsed;
                TopBorder.Visibility = Visibility.Visible;
               
            }
        }

        private void Heading_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Console.WriteLine("Click Grid!");
            if (e.ClickCount == 2)
            {
                m_IsVisible = !m_IsVisible;
                UpdateVisibility();
            }
        }
        private void Retract_Click(object sender, RoutedEventArgs e)
        {
            m_IsVisible = !m_IsVisible;
            UpdateVisibility();
        }

    }
}

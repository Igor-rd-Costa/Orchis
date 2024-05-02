using System;
using System.Windows;
using System.Windows.Input;
using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Orchis;
using OrchisEditor.View.Editor.UserControls;
using OrchisEditor.View.Editor.UserControls.ToolsPanelComponents;
using OrchisEditor.View.ProjectSelector;
using OrchisEditor.View.UserControls;

namespace OrchisEditor.View.Editor
{
    public partial class EditorWindow : Window
    {
        private ProjectSelectorWindow m_ProjectSelector;
        private static List<Action<object, MouseButtonEventArgs>> s_OnClickCallbacks = [];
        private static List<Action<object, KeyEventArgs>> s_OnKeyDownCallbacks = [];
        public EditorWindow()
        {
            m_ProjectSelector = new ProjectSelectorWindow();
            if (m_ProjectSelector.ShowDialog() == false)
            {
                Close();
                   return;
            }
            AssetManager.Init();
            SceneManager.Init();
            InitializeComponent();
            Title = Project.Name + " - Orchis Engine";
            Header.Text = Title;
            SystemMenu.Window = this;
            SceneRenderer.Attach();
            Project.OnProjectChange((hasUnsavedChanges) =>
            {
                if (hasUnsavedChanges)
                {
                    Title = "*" + Title;
                    Header.Text = Title;
                }
                else
                {
                    Title = Project.Name + " - Orchis Engine";
                    Header.Text = Title;
                }
            });
        }

        public static Outliner? GetProjectOutliner() { return ((EditorWindow)Application.Current.MainWindow).ProjectOutliner; }
        public static PropertyPanel? GetPropertyPanel() { return ((EditorWindow)Application.Current.MainWindow).PropertiesPanel; }
        public static AssetManagerView? GetAssetManagerView() { return ((EditorWindow)Application.Current.MainWindow).ToolsPanel.GetAssetManagerView(); }
        public static SceneViewer? GetSceneViewer() { return ((EditorWindow)Application.Current.MainWindow).SceneRenderer; }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.GetPosition(this).Y < WindowGrid.RowDefinitions[0].Height.Value && e.LeftButton == MouseButtonState.Pressed)
            {
                DragMove();
                Engine.Window.UpdateRect();
                return;
            }

            TriggerOnClickCallback(sender, e);
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.S && Keyboard.IsKeyDown(Key.LeftCtrl))
            {
                Project.Save();
                return;
            }

            if (Engine.IsHovering)
            {
                if (!e.IsRepeat)
                    Engine.RegisterKeyDownEvent(e.Key);
                e.Handled = true;
                return;
            }

            TriggerOnKeyDownCallback(sender, e);
        }

        private void Window_KeyUp(object sender, KeyEventArgs e)
        {
            if (Engine.IsHovering)
            {
                Engine.RegisterKeyUpEvent(e.Key);
                e.Handled= true;
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            
        }

        private void Window_MouseMove(object sender, MouseEventArgs e)
        {
            Engine.RegisterWindowHover();
        }

        public static void OnClick(Action<object, MouseButtonEventArgs> callback)
        {
            s_OnClickCallbacks.Add(callback);
        }
        public static void OnKeyDown(Action<object, KeyEventArgs> callback)
        {
            s_OnKeyDownCallbacks.Add(callback);
        }
        private void TriggerOnClickCallback(object sender, MouseButtonEventArgs e)
        {
            foreach (Action<object, MouseButtonEventArgs> callback in s_OnClickCallbacks)
            {
                if (e.Handled)
                    break;
                callback(sender, e);
            }
        }
        private void TriggerOnKeyDownCallback(object sender, KeyEventArgs e)
        {
            foreach (Action<object, KeyEventArgs> callback in s_OnKeyDownCallbacks)
            {
                if (e.Handled)
                    break;
                callback(sender, e);
            }
        }
    }
}

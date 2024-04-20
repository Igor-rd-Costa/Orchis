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

        public EditorWindow()
        {
            m_ProjectSelector = new ProjectSelectorWindow();
            if (m_ProjectSelector.ShowDialog() == false)
            {
                Close();
                return;
            }
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
        private void Window_MouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (e.GetPosition(this).Y < WindowGrid.RowDefinitions[0].Height.Value)
            {
                DragMove();
                return;
            }
        }

        private void Window_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == Key.S && Keyboard.IsKeyDown(Key.LeftCtrl))
            {
                Project.Save();
            }
        }
    }
}

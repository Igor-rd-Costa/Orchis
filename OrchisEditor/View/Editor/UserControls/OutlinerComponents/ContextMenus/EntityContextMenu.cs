using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Editor.Components;
using OrchisEditor.Controller.Orchis;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Input;

namespace OrchisEditor.View.Editor.UserControls.OutlinerComponents.ContextMenus
{
    internal class EntityContextMenu : ContextMenu
    {
        public EntityContextMenu(OutlinerTreeItem item)
        {
            DataContext = item;
            MenuItem removeItem = new()
            {
                Header = "Remove"
            };
            removeItem.PreviewMouseLeftButtonDown += Remove_MouseLeftButtonDown;
            MenuItem debug = new() { Header = "Debug" };
            debug.PreviewMouseLeftButtonDown += DebugComponents_MouseLeftButtonDown;

            MenuItem addComponentItem = new()
            {
                Header = "Add Component",
            };
            MenuItem transformComponent = new()
            {
                Header = "Tranform Component"
            };
            transformComponent.PreviewMouseLeftButtonDown += AddTransformComponent;
            addComponentItem.Items.Add(transformComponent);
            Items.Add(removeItem);
            Items.Add(addComponentItem);
            Items.Add(debug);
        }

        private void AddTransformComponent(object sender, MouseButtonEventArgs e)
        {
            
            Guid id = ((OutlinerTreeItem)DataContext).Id;
            OutlinerTreeItem parent = (OutlinerTreeItem)((OutlinerTreeItem)DataContext).Parent;
            if (parent == null)
                return;
            Guid sceneId = parent.Id;
            Scene? scene = SceneManager.GetScene(sceneId);
            Entity? entity = scene?.GetEntity(id);
            if (entity == null)
                return;
            entity.AddComponent(ComponentType.TRANSFORM);
            Outliner? outliner = EditorWindow.GetProjectOutliner();
            if (outliner == null)
                return;

            OutlinerTreeItem? item = (OutlinerTreeItem)outliner.SceneTreeView.SelectedItem;
            if (item != null && item.Id == id)
            {
                EditorWindow.GetPropertyPanel()?.UpdateGUI();
            }
        }

        private void Remove_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Scene? scene = SceneManager.GetScene(((OutlinerTreeItem)((OutlinerTreeItem)DataContext).Parent).Id);
            scene?.RemoveEntity(((OutlinerTreeItem)DataContext).Id);
        }

        private void DebugComponents_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Scene? scene = SceneManager.GetScene(((OutlinerTreeItem)((OutlinerTreeItem)DataContext).Parent).Id);
            Entity? entity = scene?.GetEntity(((OutlinerTreeItem)DataContext).Id);
            if (entity != null && scene != null)
            {
                Console.WriteLine($"Editor:\nEntity {entity.Id} has {entity.Components.Count} components.");
                OrchisInterface.OrchisEntityDebugComponents(scene.Id, entity.Id);
            }
        }
    }
}

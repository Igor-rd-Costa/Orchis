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

            MenuItem addComponentItem = new() { Header = "Add Component" };
            MenuItem transformComponent = new() { Header = "Tranform Component" };
            transformComponent.PreviewMouseLeftButtonDown += AddTransformComponent;
            MenuItem meshComponent = new() { Header = "Mesh Component" };
            meshComponent.PreviewMouseLeftButtonDown += AddMeshComponent;
            MenuItem textureComponent = new() { Header = "Texture Component" };
            textureComponent.PreviewMouseLeftButtonDown += AddTextureComponent;
            addComponentItem.Items.Add(transformComponent);
            addComponentItem.Items.Add(meshComponent);
            addComponentItem.Items.Add(textureComponent);
            Items.Add(removeItem);
            Items.Add(addComponentItem);
            Items.Add(debug);
        }

        private void AddTransformComponent(object sender, MouseButtonEventArgs e)
        {
            AddComponent(ComponentType.TRANSFORM);
        }
        private void AddMeshComponent(object sender, MouseButtonEventArgs e)
        {
            AddComponent(ComponentType.MESH);
        }
        private void AddTextureComponent(object sender, MouseButtonEventArgs e)
        {
            AddComponent(ComponentType.TEXTURE);
        }

        private void AddComponent(ComponentType componentType)
        {
            Guid id = ((OutlinerTreeItem)DataContext).Id;
            Entity? entity = SceneManager.Scene?.GetEntity(id);
            if (entity == null)
                return;
            entity.AddComponent(componentType);
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
            //Scene? scene = SceneManager.GetScene(((OutlinerTreeItem)((OutlinerTreeItem)DataContext).Parent).Id);
            //scene?.RemoveEntity(((OutlinerTreeItem)DataContext).Id);
        }

        private void DebugComponents_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            //Scene? scene = SceneManager.GetScene(((OutlinerTreeItem)((OutlinerTreeItem)DataContext).Parent).Id);
            //Entity? entity = scene?.GetEntity(((OutlinerTreeItem)DataContext).Id);
            //if (entity != null && scene != null)
            //{
            //    Console.WriteLine($"Editor:\nEntity {entity.Id} has {entity.Components.Count} components.");
            //    OrchisInterface.OrchisEntityDebugComponents(scene.Id, entity.Id);
            //}
        }
    }
}

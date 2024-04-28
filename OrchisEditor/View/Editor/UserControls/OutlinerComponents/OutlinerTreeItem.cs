using OrchisEditor.Controller.Editor;
using OrchisEditor.Controller.Orchis;
using OrchisEditor.View.Editor.UserControls.OutlinerComponents.ContextMenus;
using OrchisEditor.View.UserControls;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace OrchisEditor.View.Editor.UserControls.OutlinerComponents
{
    public enum ItemType
    {
        ITEM_TYPE_SCENE, ITEM_TYPE_ENTITY
    }

    public class OutlinerTreeItem : TreeViewItem
    {
        private readonly Guid m_ItemId;
        private string m_ItemName;
        private readonly ItemType m_Type;
        private bool m_IsInEditMode = false;
        private string m_OldName = "";

        public OutlinerTreeItem(Guid id, string name, string imageSource, ItemType type)
        {
            m_ItemName = name;
            m_ItemId = id;
            m_Type = type;
            KeyDown += OnKeyDown;
            BitmapImage bitmap = new();
            bitmap.BeginInit();
            bitmap.UriSource = new Uri(imageSource, UriKind.Relative);
            bitmap.EndInit();
            Image image = new()
            {
                Source = bitmap,
                Width = 20,
                Height = 20
            };
            TextBlock textBlock = new()
            {
                Text = m_ItemName,
                Name = "ItemText",
                TextAlignment = TextAlignment.Left,
                Background = Brushes.Transparent,
                Foreground = Brushes.White,
                Padding = new Thickness(5, 0, 0, 0)
            };
            StackPanel stackPanel = new()
            {
                Orientation = Orientation.Horizontal
            };

            stackPanel.Children.Add(image);
            stackPanel.Children.Add(textBlock);
            Header = stackPanel;

            switch (m_Type)
            {
                case ItemType.ITEM_TYPE_SCENE:
                {
                    ContextMenu = new SceneContextMenu(this);
                }
                break;
                case ItemType.ITEM_TYPE_ENTITY:
                {
                    ContextMenu = new EntityContextMenu(this);
                }
                break;
            }
        }
        public string ItemName
        {
            get { return m_ItemName; }
            set { m_ItemName = value; }
        }
        public Guid Id
        {
            get { return m_ItemId; }
        }
        public ItemType Type
        {
            get { return m_Type; }
        }
        public bool IsInEditMode
        {
            get { return m_IsInEditMode; }
        }

        private void OnKeyDown(object sender, KeyEventArgs e)
        {
            if (Engine.IsHovering)
                return;

            if (e.Key == Key.F2 && !m_IsInEditMode)
            {
                EnterEditMode();
                e.Handled = true;
            }
            if (e.Key == Key.Escape && m_IsInEditMode)
            {
                CancelEdit();
            }
            if (e.Key == Key.Enter && m_IsInEditMode)
            {
                ExitEditMode();
            }
        }
        public void EnterEditMode()
        {
            foreach (FrameworkElement e in ((StackPanel)Header).Children)
            {
                if (e.Name == "ItemText")
                {
                    TextBlock text = (TextBlock)e;
                    TextBox textBox = new()
                    {
                        Text = text.Text,
                        Name = "ItemText",
                        TextAlignment = text.TextAlignment,
                        Background = text.Background,
                        Foreground = text.Foreground,
                        Padding = text.Padding
                    };
                    m_OldName = text.Text;
                    ((StackPanel)Header).Children.Remove(e);
                    ((StackPanel)Header).Children.Add(textBox);
                    textBox.Focus();
                    break;
                }
            }
            m_IsInEditMode = true;
        }

        public void ExitEditMode()
        {

            foreach (FrameworkElement e in ((StackPanel)Header).Children)
            {
                if (e.Name == "ItemText")
                {
                    TextBox text = (TextBox)e;
                    TextBlock textBox = new()
                    {
                        Text = text.Text,
                        Name = "ItemText",
                        TextAlignment = text.TextAlignment,
                        Background = text.Background,
                        Foreground = text.Foreground,
                        Padding = text.Padding
                    };
                    bool success = ValidateNewName(text.Text) && (m_Type != ItemType.ITEM_TYPE_SCENE || QueryRename(text.Text));
                    if (success)
                    {
                        if (m_Type == ItemType.ITEM_TYPE_SCENE)
                            success = SceneManager.RenameScene(text.Text);
                        else
                            SceneManager.RenameEntity(m_ItemId, text.Text);
                    }

                    if (!success)
                    {
                        textBox.Text = m_OldName;
                    }

                    ((StackPanel)Header).Children.Remove(e);
                    ((StackPanel)Header).Children.Add(textBox);
                    break;
                }
            }
            m_IsInEditMode = false;
        }

        private void CancelEdit()
        {
            foreach (FrameworkElement e in ((StackPanel)Header).Children)
            {
                if (e.Name == "ItemText")
                {
                    TextBox text = (TextBox)e;
                    TextBlock textBox = new()
                    {
                        Text = m_OldName,
                        Name = "ItemText",
                        TextAlignment = text.TextAlignment,
                        Background = text.Background,
                        Foreground = text.Foreground,
                        Padding = text.Padding
                    };
                    ((StackPanel)Header).Children.Remove(e);
                    ((StackPanel)Header).Children.Add(textBox);
                    m_IsInEditMode = false;
                    break;
                }
            }
        }

        private bool QueryRename(string newName)
        {
            OrchisDialog dialog = new(OrchisDialogType.YES_NO);
            string type = m_Type == ItemType.ITEM_TYPE_SCENE ? "scene" : "entity";
            bool? result = dialog.ShowMessage($"Rename {type} {m_OldName} to {newName}?");
            if (result.HasValue && result.Value)
                return true;
            return false;
        }

        private bool ValidateNewName(string newName)
        {
            if (string.IsNullOrEmpty(newName)) 
                return false;
            foreach (char c in newName)
            {
                if (!char.IsLetterOrDigit(c))
                {
                    OrchisDialog dialog = new(OrchisDialogType.OK);
                    dialog.ShowMessage("Special characters are not allowed in scene/entity names.");
                    return false;
                }
            }   

            return true;
        }
    }


}

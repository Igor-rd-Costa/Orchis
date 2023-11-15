using OrchisEditor.View.Editor.UserControls.OutlinerComponents.ContextMenus;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
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
        public OutlinerTreeItem(string name, string imageSource, ItemType type)
        {
            m_ItemName = name;
            m_Type = type;
            BitmapImage bitmap = new BitmapImage();
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
                TextAlignment = TextAlignment.Left,
                Background = Brushes.Transparent,
                Foreground = Brushes.White,
                Padding = new Thickness(5,0,0,0)
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
                } break;
                case ItemType.ITEM_TYPE_ENTITY:
                {
                    ContextMenu = new EntityContextMenu(this);
                } break;
            }
        }

        private string m_ItemName;
        private readonly ItemType m_Type;

        public string ItemName
        {
            get { return m_ItemName; }
            set {
                m_ItemName = value; 
                
            }
        }

        public ItemType Type
        {
            get { return m_Type; }
        }
    }


}

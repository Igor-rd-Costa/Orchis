using OrchisEditor.Controller.Editor;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace OrchisEditor.Controller.Utils
{
    public struct TagAttribute
    {
        public TagAttribute(string name, string value)
        {
            Name = name;
            Value = value;
        }

        public string Name;
        public string Value;
    }
    public struct Tag
    {
        public Tag()
        {

        }

        public string GetAttribute(string name)
        {
            foreach (TagAttribute attr in Attributes)
            {
                if (attr.Name == name)
                    return attr.Value;
            }
            return "";
        }

        public void SetAttribute(string name, string newVal)
        {
            for (int i = 0; i < Attributes.Count; i++)
            {
                if (Attributes[i].Name == name)
                {
                    TagAttribute attr = Attributes[i];
                    attr.Value = newVal;
                    Attributes[i] = attr;
                }
            }
        }

        public string Name = "";
        public List<TagAttribute> Attributes = [];
        public List<Tag> Childs = [];
    }

    class Parser
    {
        public static Tag? ParseFileXML(string filePath)
        {
            if (!File.Exists(filePath))
            {
                //TODO handle file not found error.
                return null;
            }

            FileStream file = File.OpenRead(filePath);
            string fileContent = new StreamReader(file).ReadToEnd();
            return ParseXML(fileContent);
        }

        public static Tag? ParseXML(string xml)
        {
            xml = xml.Trim();
            if (xml.Length == 0 || xml[0] != '<')
            {
                return null;
            }
            int aaa;
            Tag? tag = GenerateTag(xml, 0, out aaa);
            return tag;
        }

        private static Tag? GenerateTag(string xml, int offset, out int newOffset)
        {
            Tag tag = new();
            newOffset = offset;
            int nameEnd = offset;
            for (; nameEnd < xml.Length; nameEnd++)
            {
                if (xml[nameEnd] == ' ' || (xml[nameEnd] == '/' && xml[nameEnd + 1] == '>') || xml[nameEnd] == '>')
                {
                    break;
                }
            }
            tag.Name = xml.Substring(offset + 1, nameEnd - offset - 1);
            if (tag.Name.StartsWith('/')) // closing tag
            {
                newOffset = nameEnd;
                return null;
            }

            if (xml[nameEnd] == ' ') // might have attributes
            {
                for (; nameEnd < xml.Length; nameEnd++)
                {
                    if ((xml[nameEnd] == ' '))
                        continue;
                    if (xml[nameEnd] == '/' && xml[nameEnd + 1] == '>') // self closing tag
                    {
                        newOffset = nameEnd;
                        return tag;
                    }
                    if (xml[nameEnd] == '>')
                        break;

                    int attrStart = nameEnd;
                    int attrEqual = xml.IndexOf('=', attrStart);
                    nameEnd = xml.IndexOf('"', attrEqual + 2);
                    string attribute = xml.Substring(attrStart, nameEnd - attrStart);
                    tag.Attributes.Add(GenerateAttribute(attribute));
                }
            } 
            else if (xml[nameEnd] == '/') // self closing tag
            {
                newOffset = nameEnd;
                return tag;
            }

            nameEnd++;
            for (; nameEnd < xml.Length; nameEnd++)
            {
                if (xml[nameEnd] == '\n' || xml[nameEnd] == '\r' || xml[nameEnd] == '\t' || xml[nameEnd] == ' ')
                    continue;
                if (xml[nameEnd] != '<') // invalid character between tags
                    return null;

                Tag? childTag = GenerateTag(xml, nameEnd, out nameEnd);
                if (!childTag.HasValue)
                {
                    nameEnd--;
                    break;
                }

                tag.Childs.Add(childTag.Value);
                nameEnd++;
            }

            newOffset = nameEnd;
            return tag;
        }
        private static TagAttribute GenerateAttribute(string nameValPair)
        {
            TagAttribute attr = new();
            string[] pair = nameValPair.Split('=');
            attr.Name = pair[0];
            attr.Value = pair[1].Substring(1, pair[1].Length - 1);

            return attr;
        }

        public static string ToXML(Tag tag)
        {
            string xml = $"<{tag.Name}";
            for (int i = 0; i < tag.Attributes.Count; i++)
            {
                xml += $" {tag.Attributes[i].Name}=\"{tag.Attributes[i].Value}\"";
            }
            if (tag.Childs.Count == 0)
            {
                xml += "/>";
                return xml;
            }
            xml += ">";
            for (int i = 0; i < tag.Childs.Count; i++)
            {
                xml += ToXML(tag.Childs[i]);
            }
            xml += $"</{tag.Name}>";
            return xml;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Formats.Asn1;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrchisEditor.Controller.Editor
{
    //struct TagAttribute
    //{
    //    public TagAttribute(string name, string value)
    //    {
    //        Name = name;
    //        Value = value;
    //    }

    //    public string Name;
    //    public string Value;
    //}

    //struct Tag
    //{
    //    public Tag()
    //    {

    //    }

    //    public string Name = "";
    //    public TagType Type = TagType.UNKNOWN_TAG;
    //    public List<TagAttribute> Attributes = [];
    //    public List<Tag> Childs = [];
    //}

    //enum TagType
    //{
    //    PROJECT_TAG, ASSETS_TAG, SCENE_TAG, ENTITY_TAG, COMPONENT_TAG, UNKNOWN_TAG
    //}

    //internal class Parser
    //{
    //    static public Tag? ParseProject(string filePath)
    //    {
    //        if (!File.Exists(filePath))
    //        {
    //            //TODO handle file not found error.
    //            return null;
    //        }

    //        FileStream file = File.OpenRead(filePath);
    //        string fileContent = new StreamReader(file).ReadToEnd();
    //        //Console.WriteLine("File:\n" + fileContent + '\n');

    //        int tagStart = fileContent.IndexOf('<', 0);
    //        int tagEnd = fileContent.IndexOf('>', tagStart);
    //        int endSpaceIndex = fileContent.IndexOf(' ', tagStart);
    //        string name = fileContent.Substring(tagStart + 1, endSpaceIndex - 1 - tagStart);
    //        int tagClose = fileContent.IndexOf("</" + name, tagEnd);
    //        return GenerateTag(fileContent, tagStart, tagClose);
    //    }


    //    static private Tag GenerateTag(string fileContent, int begin, int end)
    //    {
    //        Tag tag = new();
    //        int tagEnd = fileContent.IndexOf('>', begin);
    //        int endSpaceIndex = fileContent.IndexOf(' ', begin);
    //        if (endSpaceIndex > tagEnd)
    //            endSpaceIndex = tagEnd;
    //        tag.Name = fileContent.Substring(begin + 1, endSpaceIndex - 1 - begin);
    //        tag.Type = GetTagType(tag);
    //        int tagClose = fileContent.IndexOf("</" + tag.Name, begin);
    //        int spaceIndex;
    //        string tagContent = fileContent.Substring(begin, tagEnd - begin + 1);
    //        while (endSpaceIndex < tagEnd)
    //        {
    //            spaceIndex = endSpaceIndex;
    //            endSpaceIndex = fileContent.IndexOf(' ', spaceIndex + 1);
    //            if (endSpaceIndex > tagEnd || endSpaceIndex == -1)
    //                endSpaceIndex = tagEnd;

    //            string nameValuePair = fileContent.Substring(spaceIndex + 1, endSpaceIndex - spaceIndex - 1);
    //            int equalPos = nameValuePair.IndexOf('=');
    //            if (equalPos == -1)
    //            {
    //                //TODO handle error. Wrong file format
    //                //return null;
    //            }
    //            string name = nameValuePair.Substring(0, equalPos);
    //            string value = nameValuePair.Substring(equalPos + 1, nameValuePair.Length - equalPos - 1);
    //            if (value[0] != '"' || value[value.Length - 1] != '"')
    //            {
    //                //TODO handle error. Wrong file format
    //                //return null;
    //            }
    //            value = value.Substring(1, value.Length - 2);
    //            tag.Attributes.Add(new(name, value));
    //        }
    //        int indexTest = fileContent.IndexOf('<', tagEnd + 1);
    //        while (indexTest < tagClose || indexTest < end)
    //        {
    //            begin = fileContent.IndexOf('<', tagEnd + 1);
    //            tagEnd = fileContent.IndexOf('>', begin);
    //            endSpaceIndex = fileContent.IndexOf(' ', begin);
    //            if (endSpaceIndex > tagEnd)
    //                endSpaceIndex = tagEnd;
    //            string tagName = fileContent.Substring(begin + 1, endSpaceIndex - 1 - begin);
    //            int childTagClose = fileContent.IndexOf("</" + tagName, begin);
    //            tag.Childs.Add(GenerateTag(fileContent, begin, childTagClose));
    //            indexTest = fileContent.IndexOf('<', childTagClose + 1);
    //            tagEnd = childTagClose;
    //        }
    //        return tag;
    //    }

    //    static private TagType GetTagType(Tag tag)
    //    {
    //        switch(tag.Name)
    //        {
    //            case "Component": return TagType.COMPONENT_TAG;
    //            case "Entity": return TagType.ENTITY_TAG;
    //            case "Scene": return TagType.SCENE_TAG;
    //            case "Project": return TagType.PROJECT_TAG;
    //            case "Assets": return TagType.ASSETS_TAG;
    //            default: return TagType.UNKNOWN_TAG;
    //        }
    //    }
    //}
}

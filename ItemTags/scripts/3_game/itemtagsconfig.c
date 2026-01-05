static const string ITEM_TAGS_PROFILE_DIR = "$profile\\ItemTags\\";
static const string ITEM_TAGS_CONFIG_PATH = ITEM_TAGS_PROFILE_DIR + "tags.json";

ref ItemTagsConfig g_ItemTagsConfig;
ItemTagsConfig GetItemTagsConfig()
{
    if (!g_ItemTagsConfig)
        g_ItemTagsConfig = new ItemTagsConfig();

    return g_ItemTagsConfig;
}

class ItemTagDefinition
{
    string label;
    vector textColor;
    vector backgroundColor;

    void ItemTagDefinition(string tag = "", vector textColor = "0 0 0", vector backgroundColor = "0 0 0")
    {
        this.label = tag;
        this.textColor = textColor;
        this.backgroundColor = backgroundColor;
    }

    int GetTextColor(int defaultColor = ARGB(255, 255, 255, 255))
    {
        if (textColor != vector.Zero)
            return ARGBF(1.0, textColor[0] / 255, textColor[1] / 255, textColor[2] / 255);

        return defaultColor;
    }

    int GetBackgroundColor(int defaultColor = ARGB(200, 46, 46, 46))
    {
        if (backgroundColor != vector.Zero)
            return ARGBF(1.0, backgroundColor[0] / 255, backgroundColor[1] / 255, backgroundColor[2] / 255);

        return defaultColor;
    }
}

class ItemTagsConfig
{
    ref map<string, ref array<ref ItemTagDefinition>> tagsByClass;

    void ItemTagsConfig()
    {
        tagsByClass = new map<string, ref array<ref ItemTagDefinition>>();

        #ifdef SERVER
            LoadConfig();
        #else
            GetRPCManager().AddRPC("ItemTags", "OnItemTagsConfigReceived", this, SingleplayerExecutionType.Client);
        #endif
    }

    void LoadConfig()
    {
        if (!FileExist(ITEM_TAGS_CONFIG_PATH))
        {
            if (!FileExist(ITEM_TAGS_PROFILE_DIR))
                MakeDirectory(ITEM_TAGS_PROFILE_DIR);

            CreateDefault();
            JsonFileLoader<ItemTagsConfig>.JsonSaveFile(ITEM_TAGS_CONFIG_PATH, this);
        }
        else
        {
            JsonFileLoader<ItemTagsConfig>.JsonLoadFile(ITEM_TAGS_CONFIG_PATH, this);
        }
    }

    void CreateDefault()
    {
        ref array<ref ItemTagDefinition> sampleTags = new array<ref ItemTagDefinition>();
        sampleTags.Insert(new ItemTagDefinition("#inv_inspect_cold_lvl_two", "200 230 255", "52 115 170"));
        sampleTags.Insert(new ItemTagDefinition("#inv_inspect_hot_lvl_one", "255 255 255", "176 70 29"));
        tagsByClass.Insert("Torch", sampleTags);
    }

    ref array<ref ItemTagDefinition> GetTagsForItem(EntityAI item)
    {
        ref array<ref ItemTagDefinition> results = new array<ref ItemTagDefinition>();

        if (!item)
            return results;

        string typeName = item.GetType();

        ref array<ref ItemTagDefinition> directTags = tagsByClass.Get(typeName);
        if (directTags)
            results.InsertAll(directTags);

        MapIterator tagsIterator = tagsByClass.Begin();
        while (tagsIterator != tagsByClass.End())
        {
            string className = tagsByClass.GetIteratorKey(tagsIterator);
            if (className != typeName && item.IsKindOf(className))
            {
                ref array<ref ItemTagDefinition> inheritedTags = tagsByClass.GetIteratorElement(tagsIterator);
                if (inheritedTags)
                    results.InsertAll(inheritedTags);
            }

            tagsIterator = tagsByClass.Next(tagsIterator);
        }

        return results;
    }

    void OnItemTagsConfigReceived(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param1<ref ItemTagsConfig> param;
        if (!ctx.Read(param))
            return;

        tagsByClass = param.param1.tagsByClass;

        if (!tagsByClass)
            tagsByClass = new map<string, ref array<ref ItemTagDefinition>>();
    }
}

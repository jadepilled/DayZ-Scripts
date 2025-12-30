ref ItemRarityConfig rarityConfig;
ItemRarityConfig GetRarityConfig()
{
    if (!rarityConfig)
        rarityConfig = new ItemRarityConfig();

    return rarityConfig;
}

class ItemRarityConfig
{
    ref map<string, vector> rarityLevels;
    ref map<string, string> items;

    bool colorSlots = true;

    void ItemRarityConfig()
    {
        LoadConfig();

        #ifndef SERVER
            GetRPCManager().AddRPC("ItemRarity", "OnItemRarityConfigReceived", this, SingleplayerExecutionType.Client);
            items = new map<string, string>();
        #endif
    }

    void LoadConfig()
    {
        #ifdef SERVER
            if (!FileExist("$profile:\\ItemRarity\\config.json"))
            {
                if (!FileExist("$profile:\\ItemRarity\\"))
                    MakeDirectory("$profile:\\ItemRarity\\");
                    
                Default();

                JsonFileLoader<ItemRarityConfig>.JsonSaveFile("$profile:\\ItemRarity\\config.json", this);
            }
            else
            {
                JsonFileLoader<ItemRarityConfig>.JsonLoadFile("$profile:\\ItemRarity\\config.json", this);

                if (!rarityLevels.Contains("#STR_COMMON"))
                    rarityLevels["#STR_COMMON"] = Vector(160, 160, 160);
            }
        #endif
    }

    void Default()
    {
        rarityLevels = new map<string, vector>();

        rarityLevels["#STR_UNCOMMON"] = Vector(125, 190, 130);
        rarityLevels["#STR_RARE"] = Vector(28, 156, 228);
        rarityLevels["#STR_EPIC"] = Vector(175, 22, 213);
        rarityLevels["#STR_LEGENDARY"] = Vector(250, 145, 15);
        rarityLevels["#STR_MYTHIC"] = Vector(221, 183, 0);
        rarityLevels["#STR_EXOTIC"] = Vector(255, 7, 120);
        rarityLevels["#STR_ARTIFACT"] = Vector(255, 0, 0);

        items = new map<string, string>();

        items["CargoPants_Beige"] = "#STR_UNCOMMON";
        items["FNX45"] = "#STR_RARE";
        items["UMP45"] = "#STR_EPIC";
        items["M4A1"] = "#STR_LEGENDARY";
        items["AviatorGlasses"] = "#STR_MYTHIC";
        items["PlateCarrierVest_Black"] = "#STR_EXOTIC";
        items["Mosin9130"] = "#STR_ARTIFACT";
    }

    string GetRarity(EntityAI item)
    {
        // Retrieving element from map is the fastest, so we try that first.
        string rarity = items.Get(item.GetType());

        if (rarity)
            return rarity;

        // Item was not found in map by class name so we begin to check if any of the elements are a base class of the item.
        MapIterator itemsIterator = items.Begin();

        while (itemsIterator != items.End())
        {
            if (item.IsKindOf(items.GetIteratorKey(itemsIterator)))
                return items.GetIteratorElement(itemsIterator);

            itemsIterator = items.Next(itemsIterator);
        }

        if (!rarity)
            rarity = "#STR_COMMON";

        return rarity;
    }

    int GetRarityColor(string rarity)
    {
        vector rarityColor = rarityLevels.Get(rarity);

        if (rarityColor)
            return ARGBF(1.0, rarityColor[0] / 255, rarityColor[1] / 255, rarityColor[2] / 255);
        
        return ARGBF(1, 1, 1, 1);
    }

    bool GetColorSlots()
    {
        return colorSlots;
    }

    void OnItemRarityConfigReceived(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		Param1<ItemRarityConfig> param;

        if (!ctx.Read(param)) return;

		this.rarityLevels = param.param1.rarityLevels;
        this.items = param.param1.items;
        this.colorSlots = param.param1.colorSlots;
	}
}
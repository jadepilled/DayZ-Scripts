/**
 * ExpansionHardlineSettings.c (tier addon)
 */

class ExpansionHardlineTierSettings
{
    static const int VERSION = 1;

    int m_Version;
    ExpansionHardlineItemTier DefaultItemTier;
    bool ItemTierParentSearch;
    ref map<string, ExpansionHardlineItemTier> ItemTier;

    void ExpansionHardlineTierSettings()
    {
        m_Version = VERSION;
        ItemTier = new map<string, ExpansionHardlineItemTier>;
    }

    void Defaults()
    {
        DefaultItemTier = ExpansionHardlineItemTier.Basic;
        ItemTierParentSearch = true;

        ItemTier.Clear();
        ItemTier.Insert("itembase", ExpansionHardlineItemTier.Basic);
        ItemTier.Insert("rifle_base", ExpansionHardlineItemTier.Advanced);
        ItemTier.Insert("akm", ExpansionHardlineItemTier.Elite);
        ItemTier.Insert("expansion_l96", ExpansionHardlineItemTier.Prototype);
    }
}

modded class ExpansionHardlineSettings
{
    static const int VERSION = 12;

    ExpansionHardlineItemTier DefaultItemTier;
    bool ItemTierParentSearch;
    ref map<string, ExpansionHardlineItemTier> ItemTier = new map<string, ExpansionHardlineItemTier>;

    override bool OnRecieve(ParamsReadContext ctx)
    {
    #ifdef EXPANSIONTRACE
        auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this).Add(ctx);
    #endif

        if (!ctx.Read(PoorItemRequirement))
        {
            Error(ToString() + "::OnRecieve PoorItemRequirement");
            return false;
        }

        if (!ctx.Read(CommonItemRequirement))
        {
            Error(ToString() + "::OnRecieve CommonItemRequirement");
            return false;
        }

        if (!ctx.Read(UncommonItemRequirement))
        {
            Error(ToString() + "::OnRecieve UncommonItemRequirement");
            return false;
        }

        if (!ctx.Read(RareItemRequirement))
        {
            Error(ToString() + "::OnRecieve RareItemRequirement");
            return false;
        }

        if (!ctx.Read(EpicItemRequirement))
        {
            Error(ToString() + "::OnRecieve EpicItemRequirement");
            return false;
        }

        if (!ctx.Read(LegendaryItemRequirement))
        {
            Error(ToString() + "::OnRecieve LegendaryItemRequirement");
            return false;
        }

        if (!ctx.Read(MythicItemRequirement))
        {
            Error(ToString() + "::OnRecieve MythicItemRequirement");
            return false;
        }

        if (!ctx.Read(ExoticItemRequirement))
        {
            Error(ToString() + "::OnRecieve ExoticItemRequirement");
            return false;
        }

        if (!ctx.Read(ShowHardlineHUD))
        {
            Error(ToString() + "::OnRecieve ShowHardlineHUD");
            return false;
        }

        if (!ctx.Read(UseReputation))
        {
            Error(ToString() + "::OnRecieve UseReputation");
            return false;
        }

#ifdef EXPANSIONMODAI
        if (!ctx.Read(UseFactionReputation))
        {
            Error(ToString() + "::OnRecieve UseFactionReputation");
            return false;
        }
#endif

        if (!ctx.Read(EnableItemRarity))
        {
            Error(ToString() + "::OnRecieve EnableItemRarity");
            return false;
        }

        if (!ctx.Read(UseItemRarityOnInventoryIcons))
        {
            Error(ToString() + "::OnRecieve UseItemRarityOnInventoryIcons");
            return false;
        }

        if (!ctx.Read(UseItemRarityForMarketPurchase))
        {
            Error(ToString() + "::OnRecieve UseItemRarityForMarketPurchase");
            return false;
        }

        if (!ctx.Read(UseItemRarityForMarketSell))
        {
            Error(ToString() + "::OnRecieve UseItemRarityForMarketSell");
            return false;
        }

        if (!ctx.Read(MaxReputation))
        {
            Error(ToString() + "::OnRecieve MaxReputation");
            return false;
        }

        if (!ctx.Read(DefaultItemRarity))
        {
            Error(ToString() + "::OnRecieve DefaultItemRarity");
            return false;
        }

        if (!ctx.Read(DefaultItemTier))
        {
            Error(ToString() + "::OnRecieve DefaultItemTier");
            return false;
        }

        if (!ctx.Read(ItemTierParentSearch))
        {
            Error(ToString() + "::OnRecieve ItemTierParentSearch");
            return false;
        }

        int itemTierCount;
        if (!ctx.Read(itemTierCount))
        {
            Error(ToString() + "::OnRecieve itemTierCount");
            return false;
        }

        ItemTier.Clear();
        for (int i = 0; i < itemTierCount; i++)
        {
            string className;
            if (!ctx.Read(className))
            {
                Error(ToString() + "::OnRecieve itemTier class");
                return false;
            }

            ExpansionHardlineItemTier tier;
            if (!ctx.Read(tier))
            {
                Error(ToString() + "::OnRecieve itemTier value");
                return false;
            }

            className.ToLower();
            ItemTier.Insert(className, tier);
        }

        m_IsLoaded = true;

        ExpansionSettings.SI_Hardline.Invoke();

        return true;
    }

    override void OnSend(ParamsWriteContext ctx)
    {
        super.OnSend(ctx);

        ctx.Write(DefaultItemTier);
        ctx.Write(ItemTierParentSearch);

        ctx.Write(ItemTier.Count());
        foreach (string className, ExpansionHardlineItemTier tier: ItemTier)
        {
            ctx.Write(className);
            ctx.Write(tier);
        }
    }

    override bool OnLoad()
    {
        bool hardlineSettingsExist = super.OnLoad();

        LoadTierSettings();

        if (m_Version < VERSION)
        {
            m_Version = VERSION;
            Save();
        }

        return hardlineSettingsExist;
    }

    override bool OnSave()
    {
        bool result = super.OnSave();

        SaveTierSettings();

        return result;
    }

    override void Defaults()
    {
        super.Defaults();

        DefaultItemTier = ExpansionHardlineItemTier.Basic;
        ItemTierParentSearch = true;

        if (!ItemTier)
            ItemTier = new map<string, ExpansionHardlineItemTier>;

        ItemTier.Clear();
    }

    override bool Copy(ExpansionSettingBase setting)
    {
        ExpansionHardlineSettings s;
        if (!Class.CastTo(s, setting))
            return false;

        CopyInternal(s);
        return true;
    }

    private void CopyInternal(ExpansionHardlineSettings s)
    {
    #ifdef EXPANSIONTRACE
        auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this).Add(s);
    #endif

        PoorItemRequirement = s.PoorItemRequirement;
        CommonItemRequirement = s.CommonItemRequirement;
        UncommonItemRequirement = s.UncommonItemRequirement;
        RareItemRequirement = s.RareItemRequirement;
        EpicItemRequirement = s.EpicItemRequirement;
        LegendaryItemRequirement = s.LegendaryItemRequirement;
        MythicItemRequirement = s.MythicItemRequirement;
        ExoticItemRequirement = s.ExoticItemRequirement;

        ShowHardlineHUD = s.ShowHardlineHUD;
        UseReputation = s.UseReputation;

#ifdef EXPANSIONMODAI
        UseFactionReputation = s.UseFactionReputation;
#endif

        EnableFactionPersistence = s.EnableFactionPersistence;

        EnableItemRarity = s.EnableItemRarity;
        UseItemRarityOnInventoryIcons = s.UseItemRarityOnInventoryIcons;
        UseItemRarityForMarketPurchase = s.UseItemRarityForMarketPurchase;
        UseItemRarityForMarketSell = s.UseItemRarityForMarketSell;

        MaxReputation = s.MaxReputation;
        ReputationLossOnDeath = s.ReputationLossOnDeath;

        DefaultItemRarity = s.DefaultItemRarity;

        ItemRarityParentSearch = s.ItemRarityParentSearch;

        EntityReputation = s.EntityReputation;
        ItemRarity = s.ItemRarity;

        DefaultItemTier = s.DefaultItemTier;
        ItemTierParentSearch = s.ItemTierParentSearch;
        ItemTier = s.ItemTier;
    }

    ExpansionHardlineItemTier GetItemTierByType(string type)
    {
        type.ToLower();

        ExpansionHardlineItemTier tier;
        if (ItemTier && ItemTier.Find(type, tier))
            return tier;

        if (ItemTierParentSearch && ItemTier)
        {
            foreach (string baseClassType, ExpansionHardlineItemTier baseTier: ItemTier)
            {
                if (ExpansionStatic.Is(type, baseClassType))
                    return baseTier;
            }
        }

        return DefaultItemTier;
    }

    void AddItemTier(string type, ExpansionHardlineItemTier tier)
    {
        type.ToLower();
        ItemTier.Insert(type, tier);
    }

    protected void LoadTierSettings()
    {
        ExpansionHardlineTierSettings tierSettings = new ExpansionHardlineTierSettings();

        bool save;
        if (FileExist(EXPANSION_HARDLINE_TIER_SETTINGS))
        {
            ExpansionJsonFileParser<ExpansionHardlineTierSettings>.Load(EXPANSION_HARDLINE_TIER_SETTINGS, tierSettings);

            if (tierSettings.m_Version < ExpansionHardlineTierSettings.VERSION)
            {
                tierSettings.m_Version = ExpansionHardlineTierSettings.VERSION;
                save = true;
            }
        }
        else
        {
            tierSettings.Defaults();
            save = true;
        }

        ApplyTierSettings(tierSettings);

        if (save)
            SaveTierSettings(tierSettings);
    }

    protected void ApplyTierSettings(ExpansionHardlineTierSettings tierSettings)
    {
        DefaultItemTier = tierSettings.DefaultItemTier;
        ItemTierParentSearch = tierSettings.ItemTierParentSearch;

        if (!ItemTier)
            ItemTier = new map<string, ExpansionHardlineItemTier>;

        ItemTier.Clear();
        foreach (string className, ExpansionHardlineItemTier tier: tierSettings.ItemTier)
        {
            string lowerClassName = className;
            lowerClassName.ToLower();
            ItemTier.Insert(lowerClassName, tier);
        }
    }

    protected void SaveTierSettings(ExpansionHardlineTierSettings tierSettings = null)
    {
        if (!tierSettings)
        {
            tierSettings = new ExpansionHardlineTierSettings();
            tierSettings.DefaultItemTier = DefaultItemTier;
            tierSettings.ItemTierParentSearch = ItemTierParentSearch;

            tierSettings.ItemTier.Clear();
            tierSettings.ItemTier.Copy(ItemTier);
        }

        JsonFileLoader<ExpansionHardlineTierSettings>.JsonSaveFile(EXPANSION_HARDLINE_TIER_SETTINGS, tierSettings);
    }
}

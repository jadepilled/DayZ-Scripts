/**
 * ExpansionHardlineTierSettings.c
 */
class ExpansionHardlineTierSettings
{
    static const int VERSION = 2;

    int m_Version;
    ref map<string, int> ItemTiers;
    ref map<int, int> TierBackgroundColors;
    ref map<int, int> TierTextColors;

    void Defaults()
    {
        m_Version = VERSION;

        if (!ItemTiers)
            ItemTiers = new map<string, int>();
        else
            ItemTiers.Clear();

        ItemTiers.Insert("akm", 4);
        ItemTiers.Insert("platecarriervest", 5);
        ItemTiers.Insert("nvgoggles", 6);

        SetDefaultColors();
    }

    static ExpansionHardlineTierSettings Load()
    {
        ExpansionHardlineTierSettings settings = new ExpansionHardlineTierSettings();

        bool save;

        ExpansionStatic.MakeDirectoryRecursive(EXPANSION_HARDLINE_TIER_FOLDER);

        bool loaded = ExpansionJsonFileParser<ExpansionHardlineTierSettings>.Load(EXPANSION_HARDLINE_TIER_SETTINGS, settings);

        if (loaded)
        {
            if (settings.m_Version < VERSION)
            {
                settings.m_Version = VERSION;
                save = true;
            }
        }
        else
        {
            settings.Defaults();
            save = true;
        }

        save |= settings.Normalize();

        if (save)
            settings.Save();

        return settings;
    }

    void Save()
    {
        ExpansionStatic.MakeDirectoryRecursive(EXPANSION_HARDLINE_TIER_FOLDER);
        JsonFileLoader<ExpansionHardlineTierSettings>.JsonSaveFile(EXPANSION_HARDLINE_TIER_SETTINGS, this);
    }

    protected bool Normalize()
    {
        bool updated;

        if (!ItemTiers)
        {
            ItemTiers = new map<string, int>();
            updated = true;
        }

        map<string, int> tiers = new map<string, int>();
        tiers.Copy(ItemTiers);

        ItemTiers.Clear();
        foreach (string className, int tier: tiers)
        {
            className.ToLower();
            ItemTiers.Insert(className, Math.Clamp(tier, 0, ExpansionHardlineTierAddon.EXPANSION_MAX_TIER));
        }

        if (!TierBackgroundColors)
        {
            TierBackgroundColors = new map<int, int>();
            updated = true;
        }
        if (!TierTextColors)
        {
            TierTextColors = new map<int, int>();
            updated = true;
        }

        updated |= FillMissingColors(TierBackgroundColors, GetDefaultBackgroundColors(), ARGB(200, 20, 20, 20));
        updated |= FillMissingColors(TierTextColors, GetDefaultTextColors(), ARGB(255, 255, 255, 255));

        return updated;
    }

    protected void SetDefaultColors()
    {
        TierBackgroundColors = GetDefaultBackgroundColors();
        TierTextColors = GetDefaultTextColors();
    }

    protected map<int, int> GetDefaultBackgroundColors()
    {
        map<int, int> defaults = new map<int, int>();
        defaults.Insert(0, ARGB(200, 32, 32, 32));
        defaults.Insert(1, ExpansionHardlineItemRarityColor.Common);
        defaults.Insert(2, ExpansionHardlineItemRarityColor.Uncommon);
        defaults.Insert(3, ExpansionHardlineItemRarityColor.Rare);
        defaults.Insert(4, ExpansionHardlineItemRarityColor.Epic);
        defaults.Insert(5, ExpansionHardlineItemRarityColor.Legendary);
        defaults.Insert(6, ExpansionHardlineItemRarityColor.Mythic);
        return defaults;
    }

    protected map<int, int> GetDefaultTextColors()
    {
        map<int, int> defaults = new map<int, int>();
        defaults.Insert(0, ARGB(255, 255, 255, 255));
        defaults.Insert(1, ARGB(255, 0, 0, 0));
        defaults.Insert(2, ARGB(255, 0, 0, 0));
        defaults.Insert(3, ARGB(255, 255, 255, 255));
        defaults.Insert(4, ARGB(255, 255, 255, 255));
        defaults.Insert(5, ARGB(255, 0, 0, 0));
        defaults.Insert(6, ARGB(255, 0, 0, 0));
        return defaults;
    }

    protected bool FillMissingColors(map<int, int> colors, map<int, int> defaults, int fallback)
    {
        bool updated;
        foreach (int tier, int color: defaults)
        {
            if (!colors.Contains(tier))
            {
                colors.Insert(tier, color);
                updated = true;
            }
        }

        for (int tierIdx = 0; tierIdx <= ExpansionHardlineTierAddon.EXPANSION_MAX_TIER; tierIdx++)
        {
            int tierColor;
            if (!colors.Find(tierIdx, tierColor))
            {
                colors.Insert(tierIdx, fallback);
                updated = true;
            }
        }

        return updated;
    }

    int GetTierBackgroundColor(int tier)
    {
        return GetColorForTier(TierBackgroundColors, tier, ARGB(200, 20, 20, 20));
    }

    int GetTierTextColor(int tier)
    {
        return GetColorForTier(TierTextColors, tier, ARGB(255, 255, 255, 255));
    }

    protected int GetColorForTier(map<int, int> colors, int tier, int fallback)
    {
        tier = Math.Clamp(tier, 0, ExpansionHardlineTierAddon.EXPANSION_MAX_TIER);
        int color;
        if (colors && colors.Find(tier, color))
            return color;

        return fallback;
    }
}

/**
 * ExpansionHardlineTierSettings.c
 */
class ExpansionHardlineTierSettings
{
    static const int VERSION = 3;

    int m_Version;
    ref map<string, int> ItemTiers;
    ref map<string, int> TierBackgroundColors;
    ref map<string, int> TierTextColors;

    // Normalized lookup tables so we never index into the raw string maps during gameplay.
    protected ref array<int> m_TierBackgroundColorCache;
    protected ref array<int> m_TierTextColorCache;

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
                settings.Defaults();
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

        updated |= NormalizeColors(TierBackgroundColors, GetDefaultBackgroundColorsAsStrings(), m_TierBackgroundColorCache, ARGB(200, 20, 20, 20));
        updated |= NormalizeColors(TierTextColors, GetDefaultTextColorsAsStrings(), m_TierTextColorCache, ARGB(255, 255, 255, 255));

        return updated;
    }

    protected void SetDefaultColors()
    {
        TierBackgroundColors = GetDefaultBackgroundColorsAsStrings();
        TierTextColors = GetDefaultTextColorsAsStrings();
        m_TierBackgroundColorCache = GetDefaultBackgroundColors();
        m_TierTextColorCache = GetDefaultTextColors();
    }

    protected map<string, int> GetDefaultBackgroundColorsAsStrings()
    {
        map<string, int> defaults = new map<string, int>();
        defaults.Insert("0", ARGB(200, 32, 32, 32));
        defaults.Insert("1", ExpansionHardlineItemRarityColor.Common);
        defaults.Insert("2", ExpansionHardlineItemRarityColor.Uncommon);
        defaults.Insert("3", ExpansionHardlineItemRarityColor.Rare);
        defaults.Insert("4", ExpansionHardlineItemRarityColor.Epic);
        defaults.Insert("5", ExpansionHardlineItemRarityColor.Legendary);
        defaults.Insert("6", ExpansionHardlineItemRarityColor.Mythic);
        return defaults;
    }

    protected map<string, int> GetDefaultTextColorsAsStrings()
    {
        map<string, int> defaults = new map<string, int>();
        defaults.Insert("0", ARGB(255, 255, 255, 255));
        defaults.Insert("1", ARGB(255, 0, 0, 0));
        defaults.Insert("2", ARGB(255, 0, 0, 0));
        defaults.Insert("3", ARGB(255, 255, 255, 255));
        defaults.Insert("4", ARGB(255, 255, 255, 255));
        defaults.Insert("5", ARGB(255, 0, 0, 0));
        defaults.Insert("6", ARGB(255, 0, 0, 0));
        return defaults;
    }

    protected array<int> GetDefaultBackgroundColors()
    {
        return GetDefaultColors(GetDefaultBackgroundColorsAsStrings(), ARGB(200, 20, 20, 20));
    }

    protected array<int> GetDefaultTextColors()
    {
        return GetDefaultColors(GetDefaultTextColorsAsStrings(), ARGB(255, 255, 255, 255));
    }

    int GetTierBackgroundColor(int tier)
    {
        if (!m_TierBackgroundColorCache)
            Normalize();

        return GetColorForTier(m_TierBackgroundColorCache, tier, ARGB(200, 20, 20, 20));
    }

    int GetTierTextColor(int tier)
    {
        if (!m_TierTextColorCache)
            Normalize();

        return GetColorForTier(m_TierTextColorCache, tier, ARGB(255, 255, 255, 255));
    }

    protected bool NormalizeColors(out map<string, int> colors, map<string, int> defaults, out ref array<int> cache, int fallback)
    {
        bool updated;

        if (!colors)
        {
            colors = new map<string, int>();
            updated = true;
        }

        cache = new array<int>();
        cache.Resize(ExpansionHardlineTierAddon.EXPANSION_MAX_TIER + 1);

        array<int> defaultValues = GetDefaultColors(defaults, fallback);
        CopyColors(defaultValues, cache);

        foreach (string tierKey, int color: colors)
        {
            int tier = Math.Clamp(tierKey.ToInt(), 0, ExpansionHardlineTierAddon.EXPANSION_MAX_TIER);
            cache[tier] = color;

            string normalizedKey = tier.ToString();
            if (normalizedKey != tierKey)
            {
                colors.Remove(tierKey);
                colors.Insert(normalizedKey, color);
                updated = true;
            }
        }

        for (int tierIdx = 0; tierIdx <= ExpansionHardlineTierAddon.EXPANSION_MAX_TIER; tierIdx++)
        {
            string tierString = tierIdx.ToString();
            if (!colors.Contains(tierString))
            {
                colors.Insert(tierString, cache[tierIdx]);
                updated = true;
            }
        }

        return updated;
    }

    protected void CopyColors(array<int> source, array<int> destination)
    {
        int count = Math.Min(source.Count(), destination.Count());
        for (int i = 0; i < count; i++)
        {
            destination[i] = source[i];
        }
    }

    protected array<int> GetDefaultColors(map<string, int> defaults, int fallback)
    {
        array<int> values = new array<int>();
        values.Resize(ExpansionHardlineTierAddon.EXPANSION_MAX_TIER + 1);

        for (int tierIdx = 0; tierIdx <= ExpansionHardlineTierAddon.EXPANSION_MAX_TIER; tierIdx++)
        {
            int color;
            if (defaults && defaults.Find(tierIdx.ToString(), color))
                values[tierIdx] = color;
            else
                values[tierIdx] = fallback;
        }

        return values;
    }

    protected int GetColorForTier(array<int> colors, int tier, int fallback)
    {
        tier = Math.Clamp(tier, 0, ExpansionHardlineTierAddon.EXPANSION_MAX_TIER);
        if (colors && tier < colors.Count())
            return colors[tier];

        return fallback;
    }
}

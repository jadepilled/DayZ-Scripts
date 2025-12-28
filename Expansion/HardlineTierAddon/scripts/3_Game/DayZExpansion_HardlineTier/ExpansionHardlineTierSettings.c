/**
 * ExpansionHardlineTierSettings.c
 */
class ExpansionHardlineTierSettings
{
    static const int VERSION = 1;

    int m_Version;
    ref map<string, int> ItemTiers;

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
    }

    static ExpansionHardlineTierSettings Load()
    {
        ExpansionHardlineTierSettings settings = new ExpansionHardlineTierSettings();

        bool save;

        if (FileExist(EXPANSION_HARDLINE_TIER_SETTINGS))
        {
            ExpansionJsonFileParser<ExpansionHardlineTierSettings>.Load(EXPANSION_HARDLINE_TIER_SETTINGS, settings);

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

        settings.Normalize();

        if (save)
            settings.Save();

        return settings;
    }

    void Save()
    {
        JsonFileLoader<ExpansionHardlineTierSettings>.JsonSaveFile(EXPANSION_HARDLINE_TIER_SETTINGS, this);
    }

    protected void Normalize()
    {
        if (!ItemTiers)
            ItemTiers = new map<string, int>();

        map<string, int> tiers = new map<string, int>();
        tiers.Copy(ItemTiers);

        ItemTiers.Clear();
        foreach (string className, int tier: tiers)
        {
            className.ToLower();
            ItemTiers.Insert(className, Math.Clamp(tier, 0, ExpansionHardlineTierAddon.EXPANSION_MAX_TIER));
        }
    }
}

/**
 * ExpansionHardlineTierAddon.c
 */
static const string EXPANSION_HARDLINE_TIER_SETTINGS = EXPANSION_MISSION_SETTINGS_FOLDER + "HardlineTierSettings.json";

class ExpansionHardlineTierAddon
{
    static const int EXPANSION_MAX_TIER = 6;

    // Override table loaded from JSON so server owners can tweak tiers at runtime.
    // Keys must be lower-case class names.
    static ref map<string, int> s_ItemTierOverrides;
    static ref ExpansionHardlineTierSettings s_Settings;

    static void InitDefaults()
    {
        if (s_ItemTierOverrides)
            return;

        s_ItemTierOverrides = new map<string, int>();

        if (GetGame() && GetGame().IsServer())
        {
            s_Settings = ExpansionHardlineTierSettings.Load();
            if (s_Settings && s_Settings.ItemTiers)
                s_ItemTierOverrides.Copy(s_Settings.ItemTiers);
        }

        if (!s_ItemTierOverrides.Count())
        {
            // Example defaults: tweak or extend in your own script mod.
            s_ItemTierOverrides.Insert("akm", 4);
            s_ItemTierOverrides.Insert("platecarriervest", 5);
            s_ItemTierOverrides.Insert("nvgoggles", 6);
        }
    }

    static int GetTierForItem(string type, int rarity)
    {
        InitDefaults();

        type.ToLower();
        int tier;
        if (s_ItemTierOverrides.Find(type, tier))
            return Math.Clamp(tier, 0, EXPANSION_MAX_TIER);

        // Fallback: derive a simple tier from rarity so every item has a value.
        return Math.Clamp(rarity, 0, EXPANSION_MAX_TIER);
    }
}

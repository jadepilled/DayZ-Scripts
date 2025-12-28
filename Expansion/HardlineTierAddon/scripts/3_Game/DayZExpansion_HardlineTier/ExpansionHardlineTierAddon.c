/**
 * ExpansionHardlineTierAddon.c
 */
class ExpansionHardlineTierAddon
{
    static const int EXPANSION_MAX_TIER = 6;

    // Optional override table so server owners can set custom tiers per type.
    // Keys must be lower-case class names.
    static ref map<string, int> s_ItemTierOverrides;

    static void InitDefaults()
    {
        if (!s_ItemTierOverrides)
        {
            s_ItemTierOverrides = new map<string, int>();

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

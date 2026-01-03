/**
 * ItemBase.c (Hardline Tier Addon)
 */
modded class ItemBase
{
    protected int m_Expansion_TierLevel;

    void ItemBase()
    {
        RegisterNetSyncVariableInt("m_Expansion_TierLevel", 0, ExpansionHardlineTierAddon.EXPANSION_MAX_TIER);

        super.ItemBase();

        if (g_Game.IsServer())
        {
            ExpansionHardlineSettings hardlineSettings = GetExpansionSettings().GetHardline();
            if (hardlineSettings && hardlineSettings.EnableItemRarity)
            {
                m_Expansion_TierLevel = ExpansionHardlineTierAddon.GetTierForItem(GetType(), m_Expansion_Rarity);
            }
        }
    }

    int Expansion_GetTierLevel()
    {
        return m_Expansion_TierLevel;
    }

    void Expansion_SetTierLevel(int tier)
    {
        m_Expansion_TierLevel = Math.Clamp(tier, 0, ExpansionHardlineTierAddon.EXPANSION_MAX_TIER);

        if (g_Game.IsServer())
            SetSynchDirty();
    }
}

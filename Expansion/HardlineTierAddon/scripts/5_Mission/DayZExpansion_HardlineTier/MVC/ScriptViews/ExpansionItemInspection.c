modded class ExpansionItemInspectionBase
{
    override void UpdateItemStats()
    {
        super.UpdateItemStats();

        if (GetExpansionSettings().GetHardline().EnableItemRarity)
            Expansion_UpdateItemTier();
    }

    void Expansion_UpdateItemTier()
    {
        ItemBase itemBase;
        if (!Class.CastTo(itemBase, m_Item))
            return;

        int tierLevel = itemBase.Expansion_GetTierLevel();
        if (tierLevel <= 0)
            return;

        string text = "Tier " + tierLevel.ToString();
        ExpansionItemTooltipStatElement element = new ExpansionItemTooltipStatElement(text, ExpansionHardlineTierAddon.GetTierTextColor(tierLevel));
        m_ItemInspectionController.ItemElements.Insert(element);
    }
}

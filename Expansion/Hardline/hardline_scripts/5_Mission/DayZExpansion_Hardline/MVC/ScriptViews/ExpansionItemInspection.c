modded class ExpansionItemInspectionBase
{
	override void UpdateItemStats()
	{
		super.UpdateItemStats();

		ExpansionHardlineSettings hardlineSettings = GetExpansionSettings().GetHardline();
		if (hardlineSettings.EnableItemRarity)
		{
			UpdateItemRarity();
			UpdateItemTier(hardlineSettings);
		}
	}

	void UpdateItemRarity()
	{
		ItemBase itemBase;
		if (Class.CastTo(itemBase, m_Item))
			UpdateItemRarityEx(itemBase.Expansion_GetRarity());
	}

	void UpdateItemRarityEx(ExpansionHardlineItemRarity rarity)
	{
		if (rarity == ExpansionHardlineItemRarity.NONE)
			return;

		string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, rarity);
		string text = "#" + "STR_EXPANSION_HARDLINE_" + rarityName;
		int color;
		ExpansionStatic.GetVariableIntByName(ExpansionHardlineItemRarityColor, rarityName, color);
		ExpansionItemTooltipStatElement element = new ExpansionItemTooltipStatElement(text, color);
		m_ItemInspectionController.ItemElements.Insert(element);
	}

	void UpdateItemTier(ExpansionHardlineSettings settings)
	{
		ItemBase itemBase;
		if (settings.EnableItemRarity && Class.CastTo(itemBase, m_Item))
			UpdateItemTierEx(itemBase.Expansion_GetTier());
	}

	void UpdateItemTierEx(int tier)
	{
		if (tier < 0)
			return;

		string tierName = "Tier" + tier;
		string text = "#" + "STR_EXPANSION_HARDLINE_TIER_" + tier;
		int color;
		ExpansionStatic.GetVariableIntByName(ExpansionHardlineItemTierColor, tierName, color);
		ExpansionItemTooltipStatElement element = new ExpansionItemTooltipStatElement(text, color);
		m_ItemInspectionController.ItemElements.Insert(element);
	}
};

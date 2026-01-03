/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ItemBase
{
	protected int m_Expansion_Rarity = ExpansionHardlineItemRarity.NONE;
	protected int m_Expansion_Tier = -1;

	void ItemBase()
	{
		RegisterNetSyncVariableInt("m_Expansion_Rarity", EnumTools.GetEnumValue(ExpansionHardlineItemRarity, 0), EnumTools.GetLastEnumValue(ExpansionHardlineItemRarity));
		RegisterNetSyncVariableInt("m_Expansion_Tier", -1, 255);

		if (g_Game.IsServer() && GetExpansionSettings().GetHardline().EnableItemRarity)
		{
			m_Expansion_Rarity = GetExpansionSettings().GetHardline().GetItemRarityByType(GetType());
			m_Expansion_Tier = Expansion_GetTier();

		#ifdef EXPANSIONMODHARDLINEDEBUG
			EXTrace.Print(EXTrace.HARDLINE, this, "- Hardline item rarity: " + typename.EnumToString(ExpansionHardlineItemRarity, m_Expansion_Rarity));
		#endif
		}
	}

	ExpansionHardlineItemRarity Expansion_GetRarity()
	{
		return m_Expansion_Rarity;
	}

	int Expansion_GetTier()
	{
		return m_Expansion_Tier;
	}

	void Expansion_SetRarity(ExpansionHardlineItemRarity rarity)
	{
		m_Expansion_Rarity = rarity;

		if (g_Game.IsServer())
			SetSynchDirty();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

	#ifdef EXPANSIONMODHARDLINEDEBUG
		EXTrace.Print(EXTrace.HARDLINE, this, "- Hardline item rarity: " + typename.EnumToString(ExpansionHardlineItemRarity, m_Expansion_Rarity));
	#endif
	}
};

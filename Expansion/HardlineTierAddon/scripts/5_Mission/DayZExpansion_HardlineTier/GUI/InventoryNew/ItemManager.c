modded class ItemManager
{
    protected bool m_Expansion_HasTierLayout;

    override void Expansion_OnSettingsReceived()
    {
        super.Expansion_OnSettingsReceived();
        Expansion_InitHardlineTierWidget();
    }

    void Expansion_InitHardlineTierWidget()
    {
        auto settings = GetExpansionSettings().GetHardline(false);

        if (!m_Expansion_HasTierLayout && settings.IsLoaded() && settings.EnableItemRarity && m_TooltipWidget)
        {
            GridSpacerWidget container = GridSpacerWidget.Cast(m_TooltipWidget.FindAnyWidget("GridSpacerWidget0"));
            if (container)
            {
                g_Game.GetWorkspace().CreateWidgets("Expansion/HardlineTierAddon/gui/layouts/expansion_tooltip_entry_tier.layout", container);
                m_Expansion_HasTierLayout = true;
            }
        }
    }

    override void PrepareTooltip(EntityAI item, int x = 0, int y = 0)
    {
        super.PrepareTooltip(item, x, y);

        Expansion_InitHardlineTierWidget();

        if (m_Expansion_HasTierLayout)
        {
            InspectMenuNew.Expansion_UpdateItemInfoTier(m_TooltipWidget, item);
        }
    }
}

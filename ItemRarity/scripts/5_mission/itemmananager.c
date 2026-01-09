modded class ItemManager
{
    Widget w_ItemStatsPanel;

    Widget w_RarityPanel;
    TextWidget w_RarityText;

    #ifdef EXPANSIONMODHARDLINE
    protected bool m_Expansion_IsValidLayout;
    #endif

    void ItemManager(Widget root)
    {
        w_ItemStatsPanel = root.FindAnyWidget("GridSpacerWidget0");

        w_RarityPanel = GetGame().GetWorkspace().CreateWidgets("ItemRarity/gui/layouts/tooltip_item.layout", w_ItemStatsPanel);
        w_RarityText = TextWidget.Cast(w_RarityPanel.FindAnyWidget("ItemRarityModWidget"));

        #ifdef EXPANSIONMODHARDLINE
        m_Expansion_IsValidLayout = false;
        ExpansionSettings.SI_Hardline.Insert(Expansion_OnSettingsReceived);
        #endif
    }

    void ~ItemManager()
    {
        #ifdef EXPANSIONMODHARDLINE
        ExpansionSettings.SI_Hardline.Remove(Expansion_OnSettingsReceived);
        #endif
    }

    #ifdef EXPANSIONMODHARDLINE
    void Expansion_OnSettingsReceived()
    {
        Expansion_InitHardlineRarityWidget();
    }

    void Expansion_InitHardlineRarityWidget()
    {
        auto settings = GetExpansionSettings().GetHardline(false);

        if (!m_Expansion_IsValidLayout && settings.IsLoaded() && settings.EnableItemRarity && m_TooltipWidget)
        {
            GridSpacerWidget container = GridSpacerWidget.Cast(m_TooltipWidget.FindAnyWidget("GridSpacerWidget0"));
            if (container)
            {
                g_Game.GetWorkspace().CreateWidgets("DayZExpansion/Hardline/GUI/layouts/expansion_tooltip_entry_rarity.layout", container);
                m_Expansion_IsValidLayout = true;
            }
        }
    }
    #endif

    override void PrepareTooltip(EntityAI item, int x = 0, int y = 0)
    {
        super.PrepareTooltip(item, x, y);

        #ifdef EXPANSIONMODHARDLINE
        Expansion_InitHardlineRarityWidget();

        if (m_Expansion_IsValidLayout)
        {
            InspectMenuNew.Expansion_UpdateItemInfoRarity(m_TooltipWidget, item);
        }
        #endif

        if (item && !IsDragging())
        {
            string rarity = GetRarityConfig().GetRarity(item);
            int rarityColor = GetRarityConfig().GetRarityColor(rarity);

            w_RarityPanel.SetColor(rarityColor);
            w_RarityText.SetText(rarity);
        }
    }

    override void SetIconTemperature(EntityAI item, Widget item_w)
        {
        super.SetIconTemperature(item, item_w);

		if ( item_w && item && item.IsInherited( ItemBase ) )
		{
            int color = ColorManager.GetInstance().GetItemColor( ItemBase.Cast( item ) );

            if ( color )
            {
                Widget color_widget = item_w.FindAnyWidget( "Color" );

                if ( color <= 0 )
                {
                    ColorManager.SetBackgroundColor(color_widget, ItemBase.Cast(item));
                }
            }
		}
	}
}
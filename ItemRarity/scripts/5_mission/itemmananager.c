modded class ItemManager
{
    Widget w_ItemStatsPanel;
    
    Widget w_RarityPanel;
    TextWidget w_RarityText;

    void ItemManager(Widget root)
	{
        w_ItemStatsPanel = root.FindAnyWidget("GridSpacerWidget0");

        w_RarityPanel = GetGame().GetWorkspace().CreateWidgets("ItemRarity/gui/layouts/tooltip_item.layout", w_ItemStatsPanel);
        w_RarityText = TextWidget.Cast(w_RarityPanel.FindAnyWidget("ItemRarityWidget"));
    }

    override void PrepareTooltip(EntityAI item, int x = 0, int y = 0)
	{
		super.PrepareTooltip(item, x, y);
        
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
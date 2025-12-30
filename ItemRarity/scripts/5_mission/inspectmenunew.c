modded class InspectMenuNew
{
    Widget w_RarityPanel;
    TextWidget w_RarityText;

    override Widget Init()
	{
        Widget rootWidget = super.Init();
        Widget w_InventoryInfoPanel = rootWidget.FindAnyWidget("InventoryInfoPanelWidget");
        Widget w_LiquidType = rootWidget.FindAnyWidget("ItemLiquidTypeWidgetBackground");
        Widget w_ItemDesc = rootWidget.FindAnyWidget("ItemDescWidget");
		
        w_RarityPanel = GetGame().GetWorkspace().CreateWidgets("ItemRarity/gui/layouts/inspect_item.layout", w_InventoryInfoPanel);
        w_RarityText = TextWidget.Cast(w_RarityPanel.FindAnyWidget("ItemRarityWidget"));

        float x, y;
        w_LiquidType.GetPos(x, y);
        w_RarityPanel.SetPos(x, y);

        w_LiquidType.SetPos(x, y + 0.12);
        
        w_ItemDesc.GetPos(x, y);
        w_ItemDesc.SetPos(x, y + 0.04);

        w_ItemDesc.GetSize(x, y);
        w_ItemDesc.SetSize(x, y - 0.04);

        return rootWidget;
	}

    override void SetItem(EntityAI item)
    {
        super.SetItem(item);
    
        string rarity = GetRarityConfig().GetRarity(item);
        int rarityColor = GetRarityConfig().GetRarityColor(rarity);

        w_RarityPanel.SetColor(rarityColor);
        w_RarityText.SetText(rarity);
    }
}
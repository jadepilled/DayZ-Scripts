modded class InspectMenuNew
{
    Widget w_RarityPanel;
    TextWidget w_RarityText;

    override Widget Init()
    {
        Widget rootWidget;

        #ifdef EXPANSIONMODHARDLINE
        if (Expansion_UseCustomLayout())
        {
            layoutRoot = g_Game.GetWorkspace().CreateWidgets(Expansion_GetInspectionMenuLayout());
            rootWidget = layoutRoot;
        }
        else
        {
            rootWidget = super.Init();
        }
        #else
        rootWidget = super.Init();
        #endif

        Widget w_InventoryInfoPanel = rootWidget.FindAnyWidget("InventoryInfoPanelWidget");
        Widget w_LiquidType = rootWidget.FindAnyWidget("ItemLiquidTypeWidgetBackground");
        Widget w_ItemDesc = rootWidget.FindAnyWidget("ItemDescWidget");
        Widget w_ExistingRarity = rootWidget.FindAnyWidget("ItemRarityWidgetBackground");

        w_RarityPanel = GetGame().GetWorkspace().CreateWidgets("ItemRarity/gui/layouts/inspect_item.layout", w_InventoryInfoPanel);
        w_RarityText = TextWidget.Cast(w_RarityPanel.FindAnyWidget("ItemRarityModWidget"));

        float x, y;

        if (w_ExistingRarity)
        {
            w_ExistingRarity.GetPos(x, y);
            w_RarityPanel.SetPos(x, y + 0.04);
        }
        else if (w_LiquidType)
        {
            w_LiquidType.GetPos(x, y);
            w_RarityPanel.SetPos(x, y);

            w_LiquidType.SetPos(x, y + 0.12);

            w_ItemDesc.GetPos(x, y);
            w_ItemDesc.SetPos(x, y + 0.04);

            w_ItemDesc.GetSize(x, y);
            w_ItemDesc.SetSize(x, y - 0.04);
        }

        return rootWidget;
    }

    override void SetItem(EntityAI item)
    {
        #ifdef EXPANSIONMODHARDLINE
        if (Expansion_UseCustomLayout())
            InspectMenuNew.Expansion_UpdateItemInfoRarity(layoutRoot, item);
        #endif

        super.SetItem(item);

        string rarity = GetRarityConfig().GetRarity(item);
        int rarityColor = GetRarityConfig().GetRarityColor(rarity);

        w_RarityPanel.SetColor(rarityColor);
        w_RarityText.SetText(rarity);
    }
}
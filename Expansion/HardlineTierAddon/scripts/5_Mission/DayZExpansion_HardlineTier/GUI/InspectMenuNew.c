modded class InspectMenuNew
{
    override void SetItem(EntityAI item)
    {
        if (Expansion_UseCustomLayout())
        {
            Expansion_UpdateItemInfoTier(layoutRoot, item);
        }

        super.SetItem(item);
    }

    static void Expansion_UpdateItemInfoTier(Widget root_widget, EntityAI item)
    {
        ImageWidget tierElement = ImageWidget.Cast(root_widget.FindAnyWidget("ItemTierWidgetBackground"));
        if (!tierElement)
        {
            Widget rarityElement = root_widget.FindAnyWidget("ItemRarityWidgetBackground");
            if (rarityElement)
                tierElement = ImageWidget.Cast(g_Game.GetWorkspace().CreateWidgets("HardlineTierAddon/gui/layouts/expansion_tooltip_entry_tier.layout", rarityElement.GetParent()));
        }

        if (!tierElement)
            return;

        ItemBase itemBase = ItemBase.Cast(item);
        if (itemBase && Expansion_UseCustomLayout())
        {
            int tierLevel = itemBase.Expansion_GetTierLevel();
            if (tierLevel <= 0)
            {
                tierElement.Show(false);
                return;
            }

            string text = "Tier " + tierLevel.ToString();
            tierElement.Show(true);
            WidgetTrySetText(root_widget, "ItemTierWidget", text);
            tierElement.SetColor(ExpansionHardlineTierAddon.GetTierBackgroundColor(tierLevel));

            TextWidget tierText = TextWidget.Cast(tierElement.FindAnyWidget("ItemTierWidget"));
            if (tierText)
                tierText.SetColor(ExpansionHardlineTierAddon.GetTierTextColor(tierLevel));
            return;
        }

        tierElement.Show(false);
    }
}

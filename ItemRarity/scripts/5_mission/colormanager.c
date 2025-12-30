modded class ColorManager
{
    static void SetBackgroundColor(Widget w, ItemBase item)
    {
        if (GetRarityConfig().GetColorSlots())
        {
            string rarity = GetRarityConfig().GetRarity(item);

            if (rarity != "#STR_COMMON")
            {
                int rarityColor = GetRarityConfig().GetRarityColor(rarity);
                w.SetColor(rarityColor);
                w.SetAlpha(0.1);
                w.Show(true);
            }
            else
            {
                w.SetColor(BASE_COLOR);
            }
        }
    }
}
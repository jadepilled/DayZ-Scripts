modded class SlotsIcon
{
    #ifdef EXPANSIONMODHARDLINE
    protected ImageWidget m_Expansion_RarityColor;

    void SlotsIcon(LayoutHolder parent, Widget root, int index, EntityAI slot_parent)
    {
        m_Expansion_RarityColor = ImageWidget.Cast(g_Game.GetWorkspace().CreateWidgets("DayZExpansion/Hardline/GUI/layouts/expansion_icon_rarity.layout", m_ItemPreview));
        m_Expansion_RarityColor.Show(false);
    }
    #endif

    override void SetItemPreview()
    {
        super.SetItemPreview();

        ColorManager.SetBackgroundColor(m_ColWidget, m_Item);

        #ifdef EXPANSIONMODHARDLINE
        Expansion_UpdateItemRarity();
        #endif
    }

    override void OnIconDrop(Widget w)
    {
        super.OnIconDrop(w);

        ColorManager.SetBackgroundColor(m_ColWidget, m_Item);

        #ifdef EXPANSIONMODHARDLINE
        Expansion_UpdateItemRarity();
        #endif
    }

    #ifdef EXPANSIONMODHARDLINE
    override void Expansion_UpdateItemRarity()
    {
        Expansion_UpdateItemRarityEx(m_Item, m_Expansion_RarityColor, m_Reserved);
        ColorManager.SetBackgroundColor(m_ColWidget, m_Item);
    }

    override protected void Expansion_ResetColor()
    {
        m_Expansion_RarityColor.SetColor(m_ExpansionBaseColor);
    }
    #endif
}
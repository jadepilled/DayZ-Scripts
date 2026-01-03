modded class Icon
{
    #ifdef EXPANSIONMODHARDLINE
    protected ImageWidget m_Expansion_RarityColor;

    void Icon(LayoutHolder parent, bool hands_icon = false)
    {
        m_Expansion_RarityColor = ImageWidget.Cast(g_Game.GetWorkspace().CreateWidgets("DayZExpansion/Hardline/GUI/layouts/expansion_icon_rarity.layout", m_ItemPreview));
        m_Expansion_RarityColor.Show(false);
    }
    #endif

    override void InitEx( EntityAI obj, bool refresh = true )
    {
        super.InitEx(obj, refresh);

        #ifdef EXPANSIONMODHARDLINE
        Expansion_UpdateItemRarity();
        #endif

        m_ColorWidget.Show(true);
        Refresh();
    }

    override void CreateWhiteBackground()
    {
        super.CreateWhiteBackground();

        ColorManager.SetBackgroundColor(m_ColorWidget, m_Item);

        #ifdef EXPANSIONMODHARDLINE
        Expansion_UpdateItemRarity();
        #endif
    }

    override void FullScreen()
    {
        super.FullScreen();

        ColorManager.SetBackgroundColor(m_ColorWidget, m_Item);

        #ifdef EXPANSIONMODHARDLINE
        Expansion_UpdateItemRarity();
        #endif
    }

    override void SetItemPreviewEx(bool refresh = true)
    {
        super.SetItemPreviewEx(refresh);

        ColorManager.SetBackgroundColor(m_ColorWidget, m_Item);

        #ifdef EXPANSIONMODHARDLINE
        Expansion_UpdateItemRarity();
        #endif
    }

    #ifdef EXPANSIONMODHARDLINE
    override void Expansion_UpdateItemRarity()
    {
        Expansion_UpdateItemRarityEx(m_Item, m_Expansion_RarityColor, m_HandsIcon);
        ColorManager.SetBackgroundColor(m_ColorWidget, m_Item);
    }

    override protected void Expansion_ResetColor()
    {
        m_Expansion_RarityColor.SetColor(m_ExpansionBaseColor);
    }

    override void InitLock(EntityAI parent, EntityAI obj, int x_pos, int y_pos, bool flip)
    {
        super.InitLock(parent, obj, x_pos, y_pos, flip);
        m_Expansion_RarityColor.Show(false);
    }
    #endif
}
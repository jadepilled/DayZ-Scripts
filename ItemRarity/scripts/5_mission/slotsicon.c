modded class SlotsIcon
{
    override void SetItemPreview()
    {
        super.SetItemPreview();

        ColorManager.SetBackgroundColor(m_ColWidget, m_Item);
    }

    override void OnIconDrop(Widget w)
    {
        super.OnIconDrop(w);
        
        ColorManager.SetBackgroundColor(m_ColWidget, m_Item);
    }
}
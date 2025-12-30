modded class Icon
{
	override void InitEx( EntityAI obj, bool refresh = true )
	{
		super.InitEx(obj, refresh);

		m_ColorWidget.Show(true);
		Refresh();
	}

    override void CreateWhiteBackground()
	{
        super.CreateWhiteBackground();

        ColorManager.SetBackgroundColor(m_ColorWidget, m_Item);
	}
    
    override void FullScreen()
    {
		super.FullScreen();

		ColorManager.SetBackgroundColor(m_ColorWidget, m_Item);
	}

    override void SetItemPreviewEx(bool refresh = true)
	{
		super.SetItemPreviewEx(refresh);

        ColorManager.SetBackgroundColor(m_ColorWidget, m_Item);
	}
}
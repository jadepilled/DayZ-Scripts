modded class InspectMenuNew
{
    protected Widget m_ItemTagsRoot;
    protected Widget m_InfoPanel;
    protected Widget m_LiquidWidget;
    protected Widget m_DescWidget;
    protected ref array<Widget> m_TagWidgets;

    protected float m_DescOriginalX;
    protected float m_DescOriginalY;
    protected float m_DescOriginalW;
    protected float m_DescOriginalH;

    protected float m_LiquidOriginalX;
    protected float m_LiquidOriginalY;

    const float TAG_HORIZONTAL_PADDING = 0.01;
    const float TAG_VERTICAL_PADDING = 0.005;

    override Widget Init()
    {
        Widget rootWidget = super.Init();

        m_InfoPanel = rootWidget.FindAnyWidget("InventoryInfoPanelWidget");
        m_LiquidWidget = rootWidget.FindAnyWidget("ItemLiquidTypeWidgetBackground");
        m_DescWidget = rootWidget.FindAnyWidget("ItemDescWidget");

        m_ItemTagsRoot = GetGame().GetWorkspace().CreateWidgets("ItemTags/gui/layouts/tags_container.layout", m_InfoPanel);
        m_TagWidgets = new array<Widget>();

        if (m_DescWidget)
        {
            m_DescWidget.GetPos(m_DescOriginalX, m_DescOriginalY);
            m_DescWidget.GetSize(m_DescOriginalW, m_DescOriginalH);
        }

        if (m_LiquidWidget)
        {
            m_LiquidWidget.GetPos(m_LiquidOriginalX, m_LiquidOriginalY);
        }

        return rootWidget;
    }

    override void SetItem(EntityAI item)
    {
        super.SetItem(item);

        UpdateItemTags(item);
    }

    protected void UpdateItemTags(EntityAI item)
    {
        if (!m_ItemTagsRoot || !m_InfoPanel || !m_DescWidget)
            return;

        ClearTags();
        ResetLayout();

        if (GetGame() && GetGame().IsDedicatedServer())
            return;

        ref array<ref ItemTagDefinition> tags = GetItemTagsConfig().GetTagsForItem(item);
        if (!tags || tags.Count() == 0)
        {
            m_ItemTagsRoot.Show(false);
            return;
        }

        m_ItemTagsRoot.Show(true);

        float tagWidth = 0.18;
        float tagHeight = 0.032;

        float containerWidth;
        float containerHeight;
        m_DescWidget.GetSize(containerWidth, containerHeight);

        if (containerWidth <= 0)
            containerWidth = (tagWidth + TAG_HORIZONTAL_PADDING) * 2;

        float x = 0;
        float y = 0;
        int rows = 1;

        foreach (ItemTagDefinition tagDef : tags)
        {
            Widget tagWidget = GetGame().GetWorkspace().CreateWidgets("ItemTags/gui/layouts/item_tag.layout", m_ItemTagsRoot);
            ImageWidget background = ImageWidget.Cast(tagWidget);
            TextWidget label = TextWidget.Cast(tagWidget.FindAnyWidget("ItemTagsLabel"));

            if (background)
                background.SetColor(tagDef.GetBackgroundColor());

            if (label)
            {
                label.SetText(tagDef.label);
                label.SetColor(tagDef.GetTextColor());
            }

            if (x + tagWidth > containerWidth)
            {
                x = 0;
                y += tagHeight + TAG_VERTICAL_PADDING;
                rows++;
            }

            tagWidget.SetPos(x, y);
            tagWidget.SetSize(tagWidth, tagHeight);
            m_TagWidgets.Insert(tagWidget);

            x += tagWidth + TAG_HORIZONTAL_PADDING;
        }

        float totalHeight = rows * tagHeight + Math.Max(0, rows - 1) * TAG_VERTICAL_PADDING;
        m_ItemTagsRoot.SetPos(m_DescOriginalX, m_DescOriginalY);
        m_ItemTagsRoot.SetSize(containerWidth, totalHeight);

        float shiftDown = totalHeight + TAG_VERTICAL_PADDING;
        m_DescWidget.SetPos(m_DescOriginalX, m_DescOriginalY + shiftDown);
        m_DescWidget.SetSize(m_DescOriginalW, Math.Max(0, m_DescOriginalH - shiftDown));

        if (m_LiquidWidget)
            m_LiquidWidget.SetPos(m_LiquidOriginalX, m_LiquidOriginalY + shiftDown);
    }

    protected void ResetLayout()
    {
        m_ItemTagsRoot.Show(false);

        if (m_DescWidget)
        {
            m_DescWidget.SetPos(m_DescOriginalX, m_DescOriginalY);
            m_DescWidget.SetSize(m_DescOriginalW, m_DescOriginalH);
        }

        if (m_LiquidWidget)
            m_LiquidWidget.SetPos(m_LiquidOriginalX, m_LiquidOriginalY);
    }

    protected void ClearTags()
    {
        foreach (Widget tagWidget : m_TagWidgets)
        {
            if (tagWidget)
                tagWidget.Unlink();
        }

        m_TagWidgets.Clear();
    }
}

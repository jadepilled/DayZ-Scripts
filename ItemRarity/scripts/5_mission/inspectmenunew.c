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

	#ifdef EXPANSIONMODHARDLINE
	string Expansion_GetInspectionMenuLayout()
	{
		return "DayZExpansion/Hardline/GUI/layouts/expansion_inventory_inspect.layout";
	}

	static bool Expansion_UseCustomLayout()
	{
		ExpansionHardlineSettings settings = GetExpansionSettings().GetHardline();
		return settings && settings.EnableItemRarity;
	}

	static void Expansion_UpdateItemInfoRarity(Widget root_widget, EntityAI item)
	{
		ImageWidget rarityElement = ImageWidget.Cast(root_widget.FindAnyWidget("ItemRarityWidgetBackground"));
		if (!rarityElement)
			return;

		ItemBase itemBase = ItemBase.Cast(item);
		if (itemBase && Expansion_UseCustomLayout())
		{
			ExpansionHardlineItemRarity rarity = itemBase.Expansion_GetRarity();
			if (rarity == ExpansionHardlineItemRarity.NONE)
			{
				rarityElement.Show(false);
				return;
			}

			string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, rarity);
			string text = "#" + "STR_EXPANSION_HARDLINE_" + rarityName;
			int color;
			ExpansionStatic.GetVariableIntByName(ExpansionHardlineItemRarityColor, rarityName, color);
			rarityElement.Show(true);
			WidgetTrySetText(root_widget, "ItemRarityWidget", text, color);
			return;
		}

		ImageWidget w_ItemRarityMod = ImageWidget.Cast(root_widget.FindAnyWidget("ItemRarityModWidget"));
		if (w_ItemRarityMod)
			w_ItemRarityMod.Show(false);
		rarityElement.Show(false);
	}
	#endif
}

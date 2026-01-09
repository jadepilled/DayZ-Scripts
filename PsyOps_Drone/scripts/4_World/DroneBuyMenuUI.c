class DeliveryKitItem {
    string className;
    int quantity;

    void DeliveryKitItem(string itemClassName = "", int itemQuantity = 1) {
        className = itemClassName;
        quantity = itemQuantity;
    }
}

class DeliveryKit {
    string name;
    string kit_description;
    ref array<ref DeliveryKitItem> items;
    int price;
    bool isFree = false;
    float cooldown = 30;
    string category;

    void DeliveryKit(string kitName = "", string kitDesc = "", array<ref DeliveryKitItem> kitItems = null, int kitPrice = 0, bool kitIsFree = false, float kitCooldown = 0, string kitCategory = "") {
        name = kitName;
        kit_description = kitDesc;
        items = kitItems;

        if (!items) {
            items = new array<ref DeliveryKitItem>();
        }

        price = kitPrice;
        isFree = kitIsFree;
        cooldown = kitCooldown;
        category = kitCategory;
    }
}

class DeliveryKitCategory {
    string name;
    ref array<ref DeliveryKit> kits;

    void DeliveryKitCategory(string categoryName = "", array<ref DeliveryKit> categoryKits = null) {
        name = categoryName;
        kits = categoryKits;

        if (!kits) {
            kits = new array<ref DeliveryKit>();
        }
    }
}

class KitDataManager {
    ref array<ref DeliveryKitCategory> categories;

    void KitDataManager() {
        categories = new array<ref DeliveryKitCategory>();
    }

    bool LoadKitsFromFile(string filePath) {
        if (FileExist(filePath)) {
            JsonFileLoader<KitDataManager>.JsonLoadFile(filePath, this);
            if (!categories) {
                categories = new array<ref DeliveryKitCategory>();
            }
            foreach (DeliveryKitCategory category : categories) {
                if (!category.kits) {
                    category.kits = new array<ref DeliveryKit>();
                }
                foreach (DeliveryKit kit : category.kits) {
                    if (kit && !kit.items) {
                        kit.items = new array<ref DeliveryKitItem>();
                    }
                }
            }
            return categories.Count() > 0;
        } else {
            Print("MT_Error: Kit JSON file not found at " + filePath);
            return false;
        }
    }

    array<ref DeliveryKitCategory> GetCategories() {
        return categories;
    }

    void AddCategories(array<ref DeliveryKitCategory> categoryChunk) {
        if (!categories) {
            categories = new array<ref DeliveryKitCategory>();
        }
        foreach (DeliveryKitCategory category : categoryChunk) {
            categories.Insert(category);
        }
    }

    array<string> GetCategoryNames() {
        array<string> names = new array<string>();
        if (!categories) {
            return names;
        }
        foreach (DeliveryKitCategory category : categories) {
            if (category && category.name != "") {
                names.Insert(category.name);
            }
        }
        return names;
    }

    array<ref DeliveryKit> GetAllKits() {
        array<ref DeliveryKit> allKits = new array<ref DeliveryKit>();
        if (!categories) {
            return allKits;
        }
        foreach (DeliveryKitCategory category : categories) {
            if (!category || !category.kits) {
                continue;
            }
            foreach (DeliveryKit kit : category.kits) {
                allKits.Insert(kit);
            }
        }
        return allKits;
    }

    array<ref DeliveryKit> GetKitsForCategory(string categoryName) {
        if (!categories) {
            return new array<ref DeliveryKit>();
        }
        if (categoryName == "" || categoryName == "All") {
            return GetAllKits();
        }
        foreach (DeliveryKitCategory category : categories) {
            if (category && category.name == categoryName) {
                return category.kits;
            }
        }
        return new array<ref DeliveryKit>();
    }

    DeliveryKit FindKitByName(string kitName) {
        if (!categories) {
            return null;
        }
        foreach (DeliveryKitCategory category : categories) {
            if (!category || !category.kits) {
                continue;
            }
            foreach (DeliveryKit kit : category.kits) {
                if (kit && kit.name == kitName) {
                    return kit;
                }
            }
        }
        return null;
    }

    bool HasKits() {
        if (!categories) {
            return false;
        }
        foreach (DeliveryKitCategory category : categories) {
            if (category && category.kits && category.kits.Count() > 0) {
                return true;
            }
        }
        return false;
    }
}

class DroneBuyMenuUI : UIScriptedMenu {
    private bool m_Initialized;
    private bool m_IsMenuOpen;
    private ref KitDataManager kitDataManager;
    private MultilineTextWidget DescriptionMultilineTextWidget;
    private TextListboxWidget KitsTextListboxWidget;
    private ButtonWidget OrderButtonWidget;
    private TextWidget PriceTextWidget;
    private TextWidget PlayerCashTextWidget;
    private EditBoxWidget SearchEditBoxWidget;
    private TextListboxWidget CategoryListboxWidget;
    private WrapSpacerWidget KitItemsContentWidget;
    private string Playerbalance;
    private string m_SelectedCategory = "All";
    private string m_SearchText = "";
    private ref array<ref DeliveryKit> m_FilteredKits;
    private ref array<ref EntityAI> m_PreviewItems;

    void DroneBuyMenuUI() {
        m_FilteredKits = new array<ref DeliveryKit>();
        m_PreviewItems = new array<ref EntityAI>();
    }

    override void Update(float timeslice) {
        super.Update(timeslice);

    }

    void ~DroneBuyMenuUI() {
        CleanupMenu();
    }

    override Widget Init() {
        if (!m_Initialized) {
            layoutRoot = GetGame().GetWorkspace().CreateWidgets("drone/gui/tabletshop.layout");

            DescriptionMultilineTextWidget = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("DescriptionMultilineTextWidget"));
            KitsTextListboxWidget = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("KitsTextListboxWidget"));
            OrderButtonWidget = ButtonWidget.Cast(layoutRoot.FindAnyWidget("OrderButtonWidget"));
            PriceTextWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("PriceTextWidget"));
            PlayerCashTextWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("PlayerCashTextWidget"));
            SearchEditBoxWidget = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("SearchEditBoxWidget"));
            CategoryListboxWidget = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("CategoryListboxWidget"));
            KitItemsContentWidget = WrapSpacerWidget.Cast(layoutRoot.FindAnyWidget("KitItemsContentWidget"));

            if (!DescriptionMultilineTextWidget || !KitsTextListboxWidget || !OrderButtonWidget || !PriceTextWidget || !PlayerCashTextWidget || !SearchEditBoxWidget || !CategoryListboxWidget || !KitItemsContentWidget) {
                return null;
            }

            m_Initialized = true;
        }

        return layoutRoot;
    }

    void SetKitDataManager(KitDataManager dataManager) {
        kitDataManager = dataManager;
    }

    void InitializeMenu() {
        if (kitDataManager && kitDataManager.HasKits()) {
            PopulateCategoryList();
            PopulateKitList();
        } else {
            Print("Kits are not loaded yet.");
        }

        RequestPlayerBalance();
    }

    void RequestPlayerBalance() {
        GetRPCManager().SendRPC("drone", "GetPlayerBalanceRPC", null, true, null);
    }

    void SetPlayerBalance(int balance) {
        if (PlayerCashTextWidget) {
            Playerbalance = balance.ToString();
            PlayerCashTextWidget.SetText(balance.ToString());
        }
    }

    override void OnShow() {
        super.OnShow();
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
        GetGame().GetUIManager().ShowUICursor(true);
        GetGame().GetMission().GetHud().Show(false);
        SetMenuOpen(true);
    }

    override void OnHide() {
        super.OnHide();
        GetGame().GetUIManager().ShowUICursor(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetMission().PlayerControlEnable(true);
        GetGame().GetMission().GetHud().Show(true);
        SetMenuOpen(false);
    }

    bool IsMenuOpen() {
        return m_IsMenuOpen;
    }

    void SetMenuOpen(bool isMenuOpen) {
        m_IsMenuOpen = isMenuOpen;
        if (isMenuOpen) {
            PPEffects.SetBlurMenu(0.5);
        } else {
            PPEffects.SetBlurMenu(0);
        }
    }

    void CleanupMenu() {
        GetGame().GetDragQueue().RemoveCalls(this);
        PPEffects.SetBlurMenu(0);
        ClearKitItems();
        if (layoutRoot) {
            layoutRoot.Unlink();
        }
    }

    void PopulateCategoryList() {
        CategoryListboxWidget.ClearItems();
        CategoryListboxWidget.AddItem("All", null, 0);

        array<string> categories = kitDataManager.GetCategoryNames();
        foreach (string categoryName : categories) {
            CategoryListboxWidget.AddItem(categoryName, null, 0);
        }

        CategoryListboxWidget.SelectRow(0);
        m_SelectedCategory = "All";
    }

    void PopulateKitList() {
        KitsTextListboxWidget.ClearItems();
        m_FilteredKits.Clear();

        array<ref DeliveryKit> kits = kitDataManager.GetKitsForCategory(m_SelectedCategory);
        if (!kits || kits.Count() == 0) {
            ClearKitSelection();
            return;
        }

        string search = m_SearchText;
        search.ToLower();

        for (int i = 0; i < kits.Count(); i++) {
            DeliveryKit kit = kits[i];
            if (!kit) {
                continue;
            }

            string kitName = kit.name;
            string kitDesc = kit.kit_description;
            kitName.ToLower();
            kitDesc.ToLower();

            if (search != "" && kitName.IndexOf(search) == -1 && kitDesc.IndexOf(search) == -1) {
                continue;
            }

            string displayName = kit.name;
            if (kit.isFree) {
                displayName = "[Free] " + displayName;
            }

            m_FilteredKits.Insert(kit);
            KitsTextListboxWidget.AddItem(displayName, null, 0);
        }

        if (m_FilteredKits.Count() > 0) {
            KitsTextListboxWidget.SelectRow(0);
            UpdateSelectedKit(0);
        } else {
            ClearKitSelection();
        }
    }

    void ClearKitSelection() {
        DescriptionMultilineTextWidget.SetText("");
        PriceTextWidget.SetText("0");
        ClearKitItems();
    }

    void UpdateSelectedKit(int index) {
        if (!m_FilteredKits || m_FilteredKits.Count() == 0) {
            ClearKitSelection();
            return;
        }

        if (index >= 0 && index < m_FilteredKits.Count()) {
            DeliveryKit selectedKit = m_FilteredKits[index];
            string description = selectedKit.kit_description;

            if (selectedKit.isFree) {
                PriceTextWidget.SetText("Free");
                if (selectedKit.cooldown > 0) {
                    description += "\nCooldown: " + FormatCooldown(selectedKit.cooldown);
                }
            } else {
                PriceTextWidget.SetText(selectedKit.price.ToString());
            }

            DescriptionMultilineTextWidget.SetText(description);
            BuildKitItems(selectedKit);
        } else {
            ClearKitSelection();
        }
    }

    void BuildKitItems(DeliveryKit kit) {
        ClearKitItems();
        if (!kit || !kit.items) {
            return;
        }

        foreach (DeliveryKitItem kitItem : kit.items) {
            if (!kitItem || kitItem.className == "" || kitItem.quantity <= 0) {
                continue;
            }

            Widget itemRow = GetGame().GetWorkspace().CreateWidgets("drone/gui/kit_item_entry.layout", KitItemsContentWidget);
            if (!itemRow) {
                continue;
            }

            ItemPreviewWidget previewWidget = ItemPreviewWidget.Cast(itemRow.FindAnyWidget("KitItemPreview"));
            TextWidget nameWidget = TextWidget.Cast(itemRow.FindAnyWidget("KitItemNameText"));
            TextWidget quantityWidget = TextWidget.Cast(itemRow.FindAnyWidget("KitItemQuantityText"));

            if (nameWidget) {
                nameWidget.SetText(GetItemDisplayName(kitItem.className));
            }

            if (quantityWidget) {
                quantityWidget.SetText("x" + kitItem.quantity.ToString());
            }

            if (previewWidget) {
                EntityAI previewItem = CreatePreviewItem(kitItem.className);
                if (previewItem) {
                    previewWidget.SetItem(previewItem);
                    previewWidget.SetView(previewItem.GetViewIndex());
                    previewWidget.SetModelOrientation("0 0 0");
                }
            }
        }
    }

    string GetItemDisplayName(string className) {
        string displayName;
        if (GetGame().ConfigGetText("CfgVehicles " + className + " displayName", displayName)) {
            return displayName;
        }
        return className;
    }

    EntityAI CreatePreviewItem(string className) {
        if (className == "") {
            return null;
        }

        // Based on Scripts/5_mission/gui/scriptconsoleitemstab.c : SetPreviewObject()
        DayZGame.m_IsPreviewSpawn = true;
        EntityAI previewItem = EntityAI.Cast(GetGame().CreateObjectEx(className, "0 0 0", ECE_LOCAL | ECE_CREATEPHYSICS | ECE_TRACE));
        DayZGame.m_IsPreviewSpawn = false;

        if (previewItem) {
            dBodyDestroy(previewItem);
            previewItem.DisableSimulation(true);
            previewItem.SetAllowDamage(false);
            m_PreviewItems.Insert(previewItem);
        }

        return previewItem;
    }

    void ClearKitItems() {
        foreach (EntityAI previewItem : m_PreviewItems) {
            if (previewItem) {
                previewItem.Delete();
            }
        }
        m_PreviewItems.Clear();

        if (KitItemsContentWidget) {
            KitItemsContentWidget.DeleteChildren();
        }
    }

    string FormatCooldown(float cooldown) {
        int totalSeconds = Math.Floor(cooldown);
        int days = totalSeconds / 86400;
        int hours = (totalSeconds % 86400) / 3600;
        int minutes = (totalSeconds % 3600) / 60;
        int secs = totalSeconds % 60;
        string cooldownStr = "";
        if (days > 0) cooldownStr += days.ToString() + "d ";
        if (hours > 0) cooldownStr += hours.ToString() + "h ";
        if (minutes > 0) cooldownStr += minutes.ToString() + "m ";
        if (secs > 0) cooldownStr += secs.ToString() + "s";
        return cooldownStr;
    }

    void BuySelectedKit() {
        if (!m_FilteredKits || m_FilteredKits.Count() == 0) {
            return;
        }

        int selectedRow = KitsTextListboxWidget.GetSelectedRow();
        if (selectedRow >= 0 && selectedRow < m_FilteredKits.Count()) {
            DeliveryKit selectedKit = m_FilteredKits[selectedRow];

            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

            if (IsPlayerIndoors(player)) {
                NotificationSystem.SendNotificationToPlayerExtended(player, 5, "Purchase Failed", "You must be outdoors to make a drone purchase.", "set:dayz_gui image:icon_info");
                return;
            }

            if (!selectedKit.isFree && selectedKit.price > 0) {
                int kitPrice = selectedKit.price;
                int playerBalance = Playerbalance.ToInt();

                if (playerBalance >= kitPrice) {
                    playerBalance -= kitPrice;
                    SetPlayerBalance(playerBalance);

                } else {
                    NotificationSystem.SendNotificationToPlayerExtended(player, 5, "Insufficient Balance", "Not enough CASH to purchase the kit: " + selectedKit.name, "set:dayz_gui image:icon_info");
                    return;
                }
            }

            vector spawnPosition = player.GetPosition() + "0 300 500";
            RequestDroneSpawn(spawnPosition, selectedKit.name);

        } else {
            NotificationSystem.SendNotificationToPlayerExtended(PlayerBase.Cast(GetGame().GetPlayer()), 5, "No Kit Selected", "Please select a kit before purchasing.", "set:dayz_gui image:icon_info");
        }
    }

    bool IsPlayerIndoors(PlayerBase player) {
        vector from = player.GetPosition();
        vector to = from + "0 10 0";

        Object hitObject;
        vector hitPosition, hitNormal;
        float hitFraction;

        PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.BUILDING | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.ROADWAY;

        bool isUnderRoof = DayZPhysics.RayCastBullet(from, to, collisionLayerMask, player, hitObject, hitPosition, hitNormal, hitFraction);

        return isUnderRoof && hitObject && hitObject.IsInherited(BuildingBase);
    }

    void RequestDroneSpawn(vector spawnPosition, string kitName) {
        Param2<vector, string> data = new Param2<vector, string>(spawnPosition, kitName);
        GetRPCManager().SendRPC("drone", "SpawnDroneRPC", data, true, null);
    }

    override bool OnChange(Widget w, int x, int y, bool finished) {
        super.OnChange(w, x, y, finished);

        if (w == SearchEditBoxWidget) {
            m_SearchText = SearchEditBoxWidget.GetText();
            PopulateKitList();
            return true;
        }

        return false;
    }

    override bool OnClick(Widget w, int x, int y, int button) {
        if (!w) {
            return false;
        }

        switch (w) {
            case KitsTextListboxWidget:
                int selectedRow = KitsTextListboxWidget.GetSelectedRow();
                UpdateSelectedKit(selectedRow);
                break;
            case CategoryListboxWidget:
                int selectedCategoryRow = CategoryListboxWidget.GetSelectedRow();
                if (selectedCategoryRow < 0) {
                    return false;
                }
                string categoryName;
                CategoryListboxWidget.GetItemText(selectedCategoryRow, 0, categoryName);
                m_SelectedCategory = categoryName;
                PopulateKitList();
                break;
            case OrderButtonWidget:
                BuySelectedKit();
                break;
            default:
                return false;
        }

        return true;
    }
}

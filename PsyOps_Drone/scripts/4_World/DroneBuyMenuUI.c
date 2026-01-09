class DeliveryKitItem {
    string class_name;
    int quantity;

    void DeliveryKitItem(string itemClassName = "", int itemQuantity = 1) {
        class_name = itemClassName;
        quantity = itemQuantity;
    }
}

class DeliveryKit {
    string name;
    string kit_description;
    string category;
    ref array<ref DeliveryKitItem> items;
    int price;
    bool isFree = false;
    float cooldown = 30;

    void DeliveryKit(string kitName = "", string kitDesc = "", string kitCategory = "", array<ref DeliveryKitItem> kitItems = null, int kitPrice = 0, bool kitIsFree = false, float kitCooldown = 0) {
        name = kitName;
        kit_description = kitDesc;
        category = kitCategory;
        items = kitItems;

        if (!items) {
            items = new array<ref DeliveryKitItem>();
        }

        price = kitPrice;
        isFree = kitIsFree;
        cooldown = kitCooldown;
    }
}

class KitDataManager {
    ref array<ref DeliveryKit> kits;

    void KitDataManager() {
        kits = new array<ref DeliveryKit>();
    }

    bool LoadKitsFromFile(string filePath) {
        if (FileExist(filePath)) {
            JsonFileLoader<KitDataManager>.JsonLoadFile(filePath, this);
            return kits.Count() > 0;
        } else {
            Print("PsyOps_Drone: Kit JSON file not found at " + filePath);
            return false;
        }
    }

    array<ref DeliveryKit> GetKits() {
        return kits;
    }
}

class DroneBuyMenuUI : UIScriptedMenu {
    private bool m_Initialized;
    private bool m_IsMenuOpen;
    private ref KitDataManager kitDataManager;
    private MultilineTextWidget DescriptionMultilineTextWidget;
    private TextListboxWidget KitsTextListboxWidget;
    private WrapSpacerWidget KitItemsWrapSpacerWidget;
    private ScrollWidget KitItemsScrollWidget;
    private EditBoxWidget SearchEditBoxWidget;
    private XComboBoxWidget CategoryComboBoxWidget;
    private ButtonWidget OrderButtonWidget;
    private TextWidget PriceTextWidget;
    private EntityAI m_PreviewItem;
    private ref array<EntityAI> m_ItemPreviewEntities;
    private ref array<Widget> m_KitItemWidgets;
    private ref array<ref DeliveryKit> m_FilteredKits;
    private ref array<string> m_CategoryOptions;
    private string Playerbalance;
    private TextWidget PlayerCashTextWidget;
    private bool m_HasSelectedKit;

    void DroneBuyMenuUI() {
    }

    override void Update(float timeslice) {
        super.Update(timeslice);

    }

    void ~DroneBuyMenuUI() {
        CleanupMenu();
    }

    override Widget Init() {
        if (!m_Initialized) {
            layoutRoot = GetGame().GetWorkspace().CreateWidgets("PsyOps_Drone/gui/tabletshop.layout");

            DescriptionMultilineTextWidget = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("DescriptionMultilineTextWidget"));
            KitsTextListboxWidget = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("KitsTextListboxWidget"));
            KitItemsWrapSpacerWidget = WrapSpacerWidget.Cast(layoutRoot.FindAnyWidget("KitItemsWrapSpacerWidget"));
            KitItemsScrollWidget = ScrollWidget.Cast(layoutRoot.FindAnyWidget("KitItemsScrollWidget"));
            SearchEditBoxWidget = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("SearchEditBoxWidget"));
            CategoryComboBoxWidget = XComboBoxWidget.Cast(layoutRoot.FindAnyWidget("CategoryComboBoxWidget"));
            OrderButtonWidget = ButtonWidget.Cast(layoutRoot.FindAnyWidget("OrderButtonWidget"));
            PriceTextWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("PriceTextWidget"));
            PlayerCashTextWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("PlayerCashTextWidget"));

            if (!DescriptionMultilineTextWidget || !KitsTextListboxWidget || !KitItemsWrapSpacerWidget || !SearchEditBoxWidget || !CategoryComboBoxWidget || !OrderButtonWidget || !PriceTextWidget) {
                return null;
            }

            m_ItemPreviewEntities = new array<EntityAI>();
            m_KitItemWidgets = new array<Widget>();
            m_FilteredKits = new array<ref DeliveryKit>();
            m_CategoryOptions = new array<string>();

            m_Initialized = true;
        }

        return layoutRoot;
    }

    void SetKitDataManager(KitDataManager dataManager) {
        kitDataManager = dataManager;
    }

    void InitializeMenu() {
        if (kitDataManager && kitDataManager.GetKits().Count() > 0) {
            BuildCategoryOptions();
            ApplyFilters();
        } else {
            Print("Kits are not loaded yet.");
        }

        RequestPlayerBalance();
    }

    void RequestPlayerBalance() {
        GetRPCManager().SendRPC("psyops_drone", "GetPlayerBalanceRPC", null, true, null);
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
        if (m_PreviewItem) {
            GetGame().ObjectDelete(m_PreviewItem);
        }
        ClearKitItemsPreview();
        if (layoutRoot) {
            layoutRoot.Unlink();
        }
    }

    void BuildCategoryOptions() {
        m_CategoryOptions.Clear();
        m_CategoryOptions.Insert("All");

        array<ref DeliveryKit> kits = kitDataManager.GetKits();
        if (!kits) {
            return;
        }

        foreach (DeliveryKit kit : kits) {
            string categoryName = kit.category;
            if (categoryName == "") {
                categoryName = "Uncategorized";
            }

            if (m_CategoryOptions.Find(categoryName) == -1) {
                m_CategoryOptions.Insert(categoryName);
            }
        }

        CategoryComboBoxWidget.ClearAll();
        foreach (string categoryOption : m_CategoryOptions) {
            CategoryComboBoxWidget.AddItem(categoryOption);
        }
        CategoryComboBoxWidget.SetCurrentItem(0);
    }

    void ApplyFilters() {
        PopulateKitList();
        if (KitsTextListboxWidget.GetNumItems() > 0) {
            KitsTextListboxWidget.SelectRow(0);
            UpdateSelectedKit(0);
        } else {
            UpdateSelectedKit(-1);
        }
    }

    void PopulateKitList() {
        KitsTextListboxWidget.ClearItems();
        m_FilteredKits.Clear();

        array<ref DeliveryKit> kits = kitDataManager.GetKits();
        if (!kits || kits.Count() == 0) {
            return;
        }

        string searchFilter = SearchEditBoxWidget.GetText();
        searchFilter.ToLower();
        int categoryIndex = CategoryComboBoxWidget.GetCurrentItem();
        string selectedCategory = "All";
        if (categoryIndex >= 0 && categoryIndex < m_CategoryOptions.Count()) {
            selectedCategory = m_CategoryOptions.Get(categoryIndex);
        }

        for (int i = 0; i < kits.Count(); i++) {
            DeliveryKit kit = kits[i];
            if (!IsKitVisible(kit, searchFilter, selectedCategory)) {
                continue;
            }

            string displayName = kit.name;
            if (kit.isFree) {
                displayName = "[Free] " + displayName;
            }
            KitsTextListboxWidget.AddItem(displayName, kit, 0);
            m_FilteredKits.Insert(kit);
        }
    }

    bool IsKitVisible(DeliveryKit kit, string searchFilter, string selectedCategory) {
        if (!kit) {
            return false;
        }

        string kitName = kit.name;
        kitName.ToLower();

        if (searchFilter != "" && kitName.IndexOf(searchFilter) == -1) {
            return false;
        }

        if (selectedCategory == "All") {
            return true;
        }

        string kitCategory = kit.category;
        if (kitCategory == "") {
            kitCategory = "Uncategorized";
        }

        return kitCategory == selectedCategory;
    }

    void UpdateSelectedKit(int index) {
        if (!m_FilteredKits || m_FilteredKits.Count() == 0) {
            return;
        }

        if (index >= 0 && index < m_FilteredKits.Count()) {
            DeliveryKit selectedKit = m_FilteredKits[index];
            string description = selectedKit.kit_description;
            m_HasSelectedKit = true;

            if (selectedKit.isFree) {
                PriceTextWidget.SetText("Free");
                if (selectedKit.cooldown > 0) {
                    description += "\nCooldown: " + FormatCooldown(selectedKit.cooldown);
                }
            } else {
                PriceTextWidget.SetText(selectedKit.price.ToString());
            }

            DescriptionMultilineTextWidget.SetText(description);
            PopulateKitItems(selectedKit);
        } else {
            m_HasSelectedKit = false;
            DescriptionMultilineTextWidget.SetText("");
            PriceTextWidget.SetText("");
            ClearKitItemsPreview();
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
        if (!m_FilteredKits || m_FilteredKits.Count() == 0 || !m_HasSelectedKit) {
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
        GetRPCManager().SendRPC("psyops_drone", "SpawnDroneRPC", data, true, null);
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
            case CategoryComboBoxWidget:
                ApplyFilters();
                break;
            case OrderButtonWidget:
                BuySelectedKit();
                break;
            default:
                return false;
        }

        return true;
    }

    override bool OnChange(Widget w, int x, int y, bool finished) {
        super.OnChange(w, x, y, finished);

        if (w == SearchEditBoxWidget) {
            ApplyFilters();
            return true;
        }

        return false;
    }

    void PopulateKitItems(DeliveryKit kit) {
        ClearKitItemsPreview();

        if (!kit || !kit.items || kit.items.Count() == 0) {
            return;
        }

        foreach (DeliveryKitItem kitItem : kit.items) {
            if (!kitItem || kitItem.class_name == "") {
                continue;
            }

            Widget itemRow = GetGame().GetWorkspace().CreateWidgets("PsyOps_Drone/gui/kit_item_row.layout", KitItemsWrapSpacerWidget);
            if (!itemRow) {
                continue;
            }

            ItemPreviewWidget previewWidget = ItemPreviewWidget.Cast(itemRow.FindAnyWidget("KitItemPreviewWidget"));
            TextWidget nameWidget = TextWidget.Cast(itemRow.FindAnyWidget("KitItemNameWidget"));
            TextWidget quantityWidget = TextWidget.Cast(itemRow.FindAnyWidget("KitItemQuantityWidget"));

            EntityAI previewEntity = CreatePreviewEntity(kitItem.class_name);
            if (previewEntity && previewWidget) {
                previewWidget.SetItem(previewEntity);
                previewWidget.SetView(previewEntity.GetViewIndex());
            }

            if (nameWidget) {
                if (previewEntity) {
                    nameWidget.SetText(previewEntity.GetDisplayName());
                } else {
                    nameWidget.SetText(kitItem.class_name);
                }
            }

            if (quantityWidget) {
                int quantity = kitItem.quantity;
                if (quantity < 1) {
                    quantity = 1;
                }
                quantityWidget.SetText("x" + quantity.ToString());
            }

            if (previewEntity) {
                m_ItemPreviewEntities.Insert(previewEntity);
            }
            m_KitItemWidgets.Insert(itemRow);
        }
    }

    void ClearKitItemsPreview() {
        foreach (EntityAI previewEntity : m_ItemPreviewEntities) {
            if (previewEntity) {
                GetGame().ObjectDelete(previewEntity);
            }
        }
        m_ItemPreviewEntities.Clear();

        foreach (Widget itemWidget : m_KitItemWidgets) {
            if (itemWidget) {
                itemWidget.Unlink();
            }
        }
        m_KitItemWidgets.Clear();
    }

    EntityAI CreatePreviewEntity(string className) {
        // Based on Scripts/5_mission/gui/scriptconsoleitemstab.c : SetPreviewObject()
        if (GetGame().IsKindOf(className, "DZ_LightAI") || GetGame().IsKindOf(className, "Man")) {
            return null;
        }

        DayZGame.m_IsPreviewSpawn = true;
        EntityAI previewEntity = EntityAI.Cast(GetGame().CreateObjectEx(className, "0 0 0", ECE_LOCAL | ECE_CREATEPHYSICS | ECE_TRACE));
        DayZGame.m_IsPreviewSpawn = false;

        if (previewEntity) {
            dBodyDestroy(previewEntity);
            previewEntity.DisableSimulation(true);
            previewEntity.SetAllowDamage(false);
        }

        return previewEntity;
    }
}

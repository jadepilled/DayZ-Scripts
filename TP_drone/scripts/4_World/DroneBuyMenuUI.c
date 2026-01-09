class DeliveryKit {
    string name;
    string kit_description;
    ref array<string> items;
    int price;
    bool isFree = false;
    float cooldown = 30;

    void DeliveryKit(string kitName = "", string kitDesc = "", array<string> kitItems = null, int kitPrice = 0, bool kitIsFree = false, float kitCooldown = 0) {
        this.name = kitName;
        this.kit_description = kitDesc;
        this.items = kitItems;

        if (!this.items) {
            this.items = new array<string>();
        }

        this.price = kitPrice;
        this.isFree = kitIsFree;
        this.cooldown = kitCooldown;
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
            Print("MT_Error: Kit JSON file not found at " + filePath);
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
    private ButtonWidget OrderButtonWidget;
    private TextWidget PriceTextWidget;
    private EntityAI m_PreviewItem;
    private string Playerbalance;
    private TextWidget PlayerCashTextWidget;

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
            layoutRoot = GetGame().GetWorkspace().CreateWidgets("drone/gui/tabletshop.layout");

            DescriptionMultilineTextWidget = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("DescriptionMultilineTextWidget"));
            KitsTextListboxWidget = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("KitsTextListboxWidget"));
            OrderButtonWidget = ButtonWidget.Cast(layoutRoot.FindAnyWidget("OrderButtonWidget"));
            PriceTextWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("PriceTextWidget"));
            PlayerCashTextWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("PlayerCashTextWidget"));

            if (!DescriptionMultilineTextWidget || !KitsTextListboxWidget || !OrderButtonWidget || !PriceTextWidget) {
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
        if (kitDataManager && kitDataManager.GetKits().Count() > 0) {
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
        if (m_PreviewItem) {
            GetGame().ObjectDelete(m_PreviewItem);
        }
        if (layoutRoot) {
            layoutRoot.Unlink();
        }
    }

    void PopulateKitList() {
        KitsTextListboxWidget.ClearItems();

        array<ref DeliveryKit> kits = kitDataManager.GetKits();
        if (!kits || kits.Count() == 0) {
            return;
        }

        for (int i = 0; i < kits.Count(); i++) {
            DeliveryKit kit = kits[i];
            string displayName = kit.name;
            if (kit.isFree) {
                displayName = "[Free] " + displayName;
            }
            KitsTextListboxWidget.AddItem(displayName, null, 0);
        }
    }

    void UpdateSelectedKit(int index) {
        array<ref DeliveryKit> kits = kitDataManager.GetKits();
        if (!kits || kits.Count() == 0) {
            return;
        }

        if (index >= 0 && index < kits.Count()) {
            DeliveryKit selectedKit = kits[index];
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
        } else {
            
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
        array<ref DeliveryKit> kits = kitDataManager.GetKits();
        if (!kits || kits.Count() == 0) {
            return;
        }

        int selectedRow = KitsTextListboxWidget.GetSelectedRow();
        if (selectedRow >= 0 && selectedRow < kits.Count()) {
            DeliveryKit selectedKit = kits[selectedRow];

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

    override bool OnClick(Widget w, int x, int y, int button) {
        if (!w) {
            return false;
        }

        switch (w) {
            case KitsTextListboxWidget:
                int selectedRow = KitsTextListboxWidget.GetSelectedRow();
                UpdateSelectedKit(selectedRow);
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

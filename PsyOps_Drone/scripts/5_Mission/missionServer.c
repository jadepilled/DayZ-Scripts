class ExpansionDroneBankingData {
    string PlayerID;
    int MoneyDeposited;

    void ExpansionDroneBankingData() {
    }
}

class DC_DroneBankingData {
    string m_PlainID;
    string m_Username;
    int m_OwnedCurrency;
    int m_MaxOwnedCurrencyBonus;

    void DC_DroneBankingData() {
    }
}

class AdvancedDroneBankingData {
    string m_Steam64ID;
    string m_PlayerName;
    int m_OwnedCurrency;
    int m_PayCheckBonus;
    int m_BonusCurrency;
    string m_ClanID;

    void AdvancedDroneBankingData() {
    }
}

class LBmasterDroneBankingData {
    int version;
    string steamid;
    string playername;
    int currentMoney;
    int maxMoneyBonus;
    int paycheckBonus;
    int ignoreTransferFee;

    void LBmasterDroneBankingData() {
    }
}

class TraderPlusDroneBankingData {
    string Version;
    string SteamID64;
    string Name;
    int MoneyAmount;
    int MaxAmount;
    ref array<string> Licences;
    ref map<string, string> Insurances;

    void TraderPlusDroneBankingData() {
        Licences = new array<string>();
        Insurances = new map<string, string>();
    }
}

class PlayerKitData {
    string playerId;
    ref map<string, float> kitTimestamps; 

    void PlayerKitData(string id) {
        playerId = id;
        kitTimestamps = new map<string, float>();
    }

    void UpdateKitTimestamp(string kitName, float timestamp) {
        kitTimestamps.Set(kitName, timestamp);
    }

    float GetKitTimestamp(string kitName) {
        if (kitTimestamps.Contains(kitName)) {
            return kitTimestamps.Get(kitName);
        } else {
            return 0;
        }
    }

    void Save(string filePath) {
        JsonFileLoader<PlayerKitData>.JsonSaveFile(filePath, this);
    }

    void Load(string filePath) {
        if (FileExist(filePath)) {
            JsonFileLoader<PlayerKitData>.JsonLoadFile(filePath, this);
        }
    }
}

class PlayerKitDataManager {
    ref map<string, ref PlayerKitData> playerDataMap;

    void PlayerKitDataManager() {
        playerDataMap = new map<string, ref PlayerKitData>();
    }

    PlayerKitData GetPlayerData(string playerId) {
        if (!playerDataMap.Contains(playerId)) {
            string filePath = GetPlayerDataFilePath(playerId);
            PlayerKitData data = new PlayerKitData(playerId);
            data.Load(filePath);
            playerDataMap.Insert(playerId, data);
        }
        return playerDataMap.Get(playerId);
    }

    void SavePlayerData(string playerId) {
        if (playerDataMap.Contains(playerId)) {
            PlayerKitData data = playerDataMap.Get(playerId);
            string filePath = GetPlayerDataFilePath(playerId);
            data.Save(filePath);
        }
    }

    string GetPlayerDataFilePath(string playerId) {
        string folderPath = "$profile:\\PsyOps_Drone\\PlayerData\\";
        if (!FileExist(folderPath)) {
            MakeDirectory(folderPath);
        }
        return folderPath + playerId + ".json";
    }
}


modded class MissionServer {
    private ref KitDataManager kitDataManager;
    private ref PlayerKitDataManager playerKitDataManager;

    override void OnInit() {
        super.OnInit();

        GetRPCManager().AddRPC("drone", "SpawnDroneRPC", this, SingeplayerExecutionType.Server);
        GetRPCManager().AddRPC("drone", "SendKitsToClient", this, SingeplayerExecutionType.Server);
        GetRPCManager().AddRPC("drone", "GetPlayerBalanceRPC", this, SingeplayerExecutionType.Server);

        playerKitDataManager = new PlayerKitDataManager();

        LoadDroneKitsConfig();
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LoadDroneKitsConfig, 8000, false);
    }

    void LoadDroneKitsConfig() {
        string folderPath = "$profile:\\PsyOps_Drone";
        string filePath = folderPath + "\\kits.json";

        if (!FileExist(folderPath)) {
            Print("PsyOps_Drone folder does NOT exist at: " + folderPath + ". Creating folder.");
            MakeDirectory(folderPath);
        }

        if (!FileExist(filePath)) {
            Print("kits.json file does NOT exist at: " + filePath);

            KitDataManager defaultKitDataManager = new KitDataManager();
            defaultKitDataManager.categories = CreateDefaultKits();
            JsonFileLoader<KitDataManager>.JsonSaveFile(filePath, defaultKitDataManager);

            Print("Default kits.json file created at: " + filePath);
        }

        kitDataManager = new KitDataManager();
        bool loaded = kitDataManager.LoadKitsFromFile(filePath);

        if (!loaded) {
            Print("MT_Error: Failed to load kits from JSON on the server.");
        } else {
            Print("Successfully loaded kits on the server. Number of categories loaded: " + kitDataManager.GetCategories().Count());
        }
    }

	ref array<ref DeliveryKitCategory> CreateDefaultKits() {
		ref array<ref DeliveryKitCategory> defaultCategories = new array<ref DeliveryKitCategory>();

        ref array<ref DeliveryKitItem> dailyItems = new array<ref DeliveryKitItem>();
        dailyItems.Insert(new DeliveryKitItem("BandageDressing", 2));
        dailyItems.Insert(new DeliveryKitItem("WaterBottle", 1));
        dailyItems.Insert(new DeliveryKitItem("Matchbox", 1));
        ref array<ref DeliveryKit> dailyKits = new array<ref DeliveryKit>();
        dailyKits.Insert(new DeliveryKit("Daily Supplies", "Free Daily Crate Contains BandageDressing, WaterBottle, Matchbox", dailyItems, 0, true, 86400, "Daily"));
        defaultCategories.Insert(new DeliveryKitCategory("Daily", dailyKits));

        ref array<ref DeliveryKitItem> medicalItems = new array<ref DeliveryKitItem>();
        medicalItems.Insert(new DeliveryKitItem("FirstAidKit", 1));
        medicalItems.Insert(new DeliveryKitItem("BandageDressing", 2));
        medicalItems.Insert(new DeliveryKitItem("Morphine", 1));
        medicalItems.Insert(new DeliveryKitItem("SalineBagIV", 1));
        ref array<ref DeliveryKit> medicalKits = new array<ref DeliveryKit>();
        medicalKits.Insert(new DeliveryKit("Medical Supplies", "This kit contains: FirstAidKit, BandageDressing, Morphine, SalineBagIV", medicalItems, 6500, false, 60, "Medical"));
        defaultCategories.Insert(new DeliveryKitCategory("Medical", medicalKits));

        ref array<ref DeliveryKitItem> buildingItems = new array<ref DeliveryKitItem>();
        buildingItems.Insert(new DeliveryKitItem("WoodenPlank", 10));
        buildingItems.Insert(new DeliveryKitItem("NailBox", 1));
        buildingItems.Insert(new DeliveryKitItem("Hammer", 1));
        ref array<ref DeliveryKit> buildingKits = new array<ref DeliveryKit>();
        buildingKits.Insert(new DeliveryKit("Basic Building Supplies", "This kit contains: WoodenPlank, NailBox, Hammer", buildingItems, 1500, false, 60, "Building"));
        defaultCategories.Insert(new DeliveryKitCategory("Building", buildingKits));

		return defaultCategories;
	}


    void GetPlayerBalanceRPC(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target) {
        if (type == CallType.Server && sender) {
            int playerBalance = 0;
            bool success = false;

            #ifdef DC_Banking
                success = GetDCBankingBalance(sender, playerBalance);
            #endif

            #ifdef KR_Banking
                if (!success) {
                    success = GetKRBankingBalance(sender, playerBalance);
                }
            #endif

            #ifdef LBmaster_Groups_ATM
                if (!success) {
                    success = GetLBmasterBankingBalance(sender, playerBalance);
                }
            #endif

            #ifdef TraderPlus
                if (!success) {
                    success = GetTraderPlusBankingBalance(sender, playerBalance);
                }
            #endif

            // ExpansionBanking
            if (!success) {
                success = GetExpansionBankingBalance(sender, playerBalance);
            }

            if (success) {
                Param1<int> data = new Param1<int>(playerBalance);
                GetRPCManager().SendRPC("drone", "ReceivePlayerBalanceRPC", data, true, sender);
            } else {
                Print("MT_Error: Failed to get player balance for " + sender.GetName());
            }
        }
    }

    bool GetTraderPlusBankingBalance(PlayerIdentity sender, out int balance) {
        string steam64ID = sender.GetPlainId();
        string bankFilePath = "$profile:\\TraderPlus\\TraderPlusBankDatabase\\" + "Account_" + steam64ID + ".json";

        if (!FileExist(bankFilePath)) {
            balance = 0;
            return false;
        }

        TraderPlusDroneBankingData bankData;
        JsonFileLoader<TraderPlusDroneBankingData>.JsonLoadFile(bankFilePath, bankData);

        if (!bankData) {
            balance = 0;
            return false;
        }

        balance = bankData.MoneyAmount;
        return true;
    }

    bool GetDCBankingBalance(PlayerIdentity sender, out int balance) {
        string steam64ID = sender.GetPlainId();
        string bankFilePath = "$profile:\\DC_Banking\\PlayerDatabase\\" + steam64ID + ".json";

        if (!FileExist(bankFilePath)) {
            balance = 0;
            return false;
        }

        DC_DroneBankingData bankData;
        JsonFileLoader<DC_DroneBankingData>.JsonLoadFile(bankFilePath, bankData);

        if (!bankData) {
            balance = 0;
            return false;
        }

        balance = bankData.m_OwnedCurrency;
        return true;
    }

    bool GetExpansionBankingBalance(PlayerIdentity sender, out int balance) {
        string playerID = sender.GetId();
        string bankFilePath = "$profile:\\ExpansionMod\\ATM\\" + playerID + ".json";

        if (!FileExist(bankFilePath)) {
            balance = 0;
            return false;
        }

        ExpansionDroneBankingData bankData;
        JsonFileLoader<ExpansionDroneBankingData>.JsonLoadFile(bankFilePath, bankData);

        if (!bankData) {
            balance = 0;
            return false;
        }

        balance = bankData.MoneyDeposited;
        return true;
    }

    bool GetKRBankingBalance(PlayerIdentity sender, out int balance) {
        string steam64ID = sender.GetPlainId();
        string bankFilePath = "$profile:\\KR_BANKING\\PlayerDataBase\\" + steam64ID + ".json";

        if (!FileExist(bankFilePath)) {
            balance = 0;
            return false;
        }

        AdvancedDroneBankingData bankData;
        JsonFileLoader<AdvancedDroneBankingData>.JsonLoadFile(bankFilePath, bankData);

        if (!bankData) {
            balance = 0;
            return false;
        }

        balance = bankData.m_OwnedCurrency;
        return true;
    }

    bool GetLBmasterBankingBalance(PlayerIdentity sender, out int balance) {
        string steam64ID = sender.GetPlainId();
        string bankFilePath = "$profile:\\LBmaster\\Data\\LBBanking\\Players\\" + steam64ID + ".json";

        if (!FileExist(bankFilePath)) {
            balance = 0;
            return false;
        }

        LBmasterDroneBankingData bankData;
        JsonFileLoader<LBmasterDroneBankingData>.JsonLoadFile(bankFilePath, bankData);

        if (!bankData) {
            balance = 0;
            return false;
        }

        balance = bankData.currentMoney;
        return true;
    }

    void SendKitsToClient(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target) {
        if (type == CallType.Server && sender) {
            int chunkSize = 1;  // Number of categories to send per RPC
            int totalCategories = kitDataManager.GetCategories().Count();

            for (int i = 0; i < totalCategories; i += chunkSize) {
                array<ref DeliveryKitCategory> categoryChunk = new array<ref DeliveryKitCategory>();

                for (int j = i; j < Math.Min(i + chunkSize, totalCategories); j++) {
                    categoryChunk.Insert(kitDataManager.GetCategories().Get(j));
                }

                string kitsJson;
                JsonSerializer js = new JsonSerializer();
                js.WriteToString(categoryChunk, false, kitsJson);

                Param1<string> data = new Param1<string>(kitsJson);
                GetRPCManager().SendRPC("drone", "ReceiveKitsFromServer", data, false, sender);
            }
        }
    }

    void SendNotificationToPlayer(PlayerBase player, string title, string message, string icon) {
        if (player && message != "") {
            NotificationSystem.SendNotificationToPlayerExtended(player, 5, title, message, icon);
        }
    }

float GetUnixTime()
{
    int year, month, day, hour, minute, second;
    GetYearMonthDayUTC(year, month, day);
    GetHourMinuteSecondUTC(hour, minute, second);

    int totalDays = DaysSinceEpoch(year, month, day);

    float unixTime = totalDays * 86400 + hour * 3600 + minute * 60 + second;

    return unixTime;
}

int DaysSinceEpoch(int year, int month, int day)
{
    int totalDays = 0;

    for (int y = 1970; y < year; y++)
    {
        if (IsLeapYear(y))
        {
            totalDays += 366;
        }
        else
        {
            totalDays += 365;
        }
    }

    int daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    if (IsLeapYear(year))
    {
        daysInMonth[1] = 29; 
    }

    for (int m = 1; m < month; m++)
    {
        totalDays += daysInMonth[m - 1];
    }

    totalDays += day - 1; 

    return totalDays;
}

bool IsLeapYear(int year)
{
    if ((year % 4) != 0)
    {
        return false;
    }
    else if ((year % 100) != 0)
    {
        return true;
    }
    else if ((year % 400) != 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void SpawnDroneRPC(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
{
    if (type == CallType.Server && sender)
    {
        Param2<vector, string> data;
        if (!ctx.Read(data))
        {
            Print("MT_Error: Failed to read RPC data.");
            return;
        }

        vector spawnPosition = data.param1;
        string kitName = data.param2;

        PlayerBase targetPlayer = GetPlayerByIdentity(sender);
        if (!targetPlayer)
        {
            Print("MT_Error: Could not find PlayerBase for sender " + sender.GetName());
            return;
        }

        // Find the selected kit
        DeliveryKit selectedKit = kitDataManager.FindKitByName(kitName);

        if (!selectedKit)
        {
            Print("MT_Error: Player " + sender.GetName() + " attempted to spawn an invalid kit: " + kitName);
            SendNotificationToPlayer(targetPlayer, "Drone Store Info", "Invalid kit selected.", "set:dayz_gui image:icon_error");
            return;
        }

        PlayerKitData playerData = playerKitDataManager.GetPlayerData(sender.GetId());
        if (selectedKit.cooldown > 0)
        {
            float lastClaimTime = playerData.GetKitTimestamp(kitName);
            float currentTime = GetUnixTime();//just use cf

            if (currentTime - lastClaimTime < selectedKit.cooldown)
            {
                float timeRemaining = selectedKit.cooldown - (currentTime - lastClaimTime);
                SendNotificationToPlayer(targetPlayer, "Drone Store Info", "You need to wait " + TimeToString(timeRemaining) + " before claiming this kit again.", "set:dayz_gui image:icon_error");
                return;
            }
        }

        if (selectedKit.price > 0)
        {
            int kitPrice = selectedKit.price;
            bool hasSufficientFunds = false;

            #ifdef DC_Banking
                hasSufficientFunds = ProcessDCBanking(sender, targetPlayer, kitPrice);
            #endif

            #ifdef KR_Banking
                if (!hasSufficientFunds)
                {
                    hasSufficientFunds = ProcessKR_Banking(sender, targetPlayer, kitPrice);
                }
            #endif

            #ifdef LBmaster_Groups_ATM
                if (!hasSufficientFunds)
                {
                    hasSufficientFunds = ProcessLBmasterBanking(sender, targetPlayer, kitPrice);
                }
            #endif

            #ifdef TraderPlus
                if (!hasSufficientFunds)
                {
                    hasSufficientFunds = ProcessTraderPlusBanking(sender, targetPlayer, kitPrice);
                }
            #endif

            // ExpansionBanking
            if (!hasSufficientFunds)
            {
                hasSufficientFunds = ProcessExpansionBanking(sender, targetPlayer, kitPrice);
            }

            if (!hasSufficientFunds)
            {
                SendNotificationToPlayer(targetPlayer, "Drone Store Info", "Insufficient funds or banking mod not found.", "set:dayz_gui image:icon_error");
                return;
            }
        }

       

	    playerData = playerKitDataManager.GetPlayerData(sender.GetId());
        float currentTimestamp = GetUnixTime();
        playerData.UpdateKitTimestamp(kitName, currentTimestamp);
        playerKitDataManager.SavePlayerData(sender.GetId());

        string purchaseMessage;
        if (selectedKit.price > 0)
        {
            purchaseMessage = "Purchased " + kitName + " for " + selectedKit.price + ".";
        }
        else
        {
            purchaseMessage = "You have received " + kitName + ".";
        }
        SendNotificationToPlayer(targetPlayer, "Drone Store Info", purchaseMessage, "set:dayz_gui image:icon_success");

        ref array<string> lootItems = new array<string>();
        foreach (DeliveryKitItem kitItem : selectedKit.items)
        {
            if (!kitItem || kitItem.className == "" || kitItem.quantity <= 0)
            {
                continue;
            }

            for (int i = 0; i < kitItem.quantity; i++)
            {
                lootItems.Insert(kitItem.className);
            }
        }

        DroneSpawner spawner = new DroneSpawner();
        spawner.SpawnDrone(targetPlayer, spawnPosition, lootItems);
    }
}


string TimeToString(float seconds) {
    int totalSeconds = Math.Floor(seconds);
    int days = totalSeconds / 86400;
    int hours = (totalSeconds % 86400) / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int secs = totalSeconds % 60;
    string timeStr = "";
    if (days > 0) timeStr += days.ToString() + "d ";
    if (hours > 0) timeStr += hours.ToString() + "h ";
    if (minutes > 0) timeStr += minutes.ToString() + "m ";
    if (secs > 0) timeStr += secs.ToString() + "s";
    return timeStr;
}


bool ProcessTraderPlusBanking(PlayerIdentity sender, PlayerBase targetPlayer, int kitPrice) {
    string steam64ID = sender.GetPlainId();
    string bankFilePath = "$profile:\\TraderPlus\\TraderPlusBankDatabase\\Account_" + steam64ID + ".json";

    if (!FileExist(bankFilePath)) {
        Print("[DroneStore] Bank file not found for player " + sender.GetName() + " at " + bankFilePath);
        SendNotificationToPlayer(targetPlayer, "Drone Store", "Bank data not found. Cannot complete purchase.", "set:dayz_gui image:icon_error");
        return false;
    }

    TraderPlusDroneBankingData bankData;
    JsonFileLoader<TraderPlusDroneBankingData>.JsonLoadFile(bankFilePath, bankData);

    if (!bankData) {
        Print("[DroneStore] Failed to load bank data for player " + sender.GetName());
        SendNotificationToPlayer(targetPlayer, "Drone Store", "Error loading bank data. Cannot complete purchase.", "set:dayz_gui image:icon_error");
        return false;
    }

    if (bankData.MoneyAmount < kitPrice) {
        Print("[DroneStore] Player " + sender.GetName() + " has insufficient funds.");
        SendNotificationToPlayer(targetPlayer, "Drone Store", "You don't have enough money!", "set:dayz_gui image:icon_warning");
        return false;
    }

    bankData.MoneyAmount -= kitPrice;
    JsonFileLoader<TraderPlusDroneBankingData>.JsonSaveFile(bankFilePath, bankData);

    return true;
}

bool ProcessDCBanking(PlayerIdentity sender, PlayerBase targetPlayer, int kitPrice) {
    string steam64ID = sender.GetPlainId();
    string bankFilePath = "$profile:\\DC_Banking\\PlayerDatabase\\" + steam64ID + ".json";

    if (!FileExist(bankFilePath)) {
        Print("[DroneStore] Bank file not found for player " + sender.GetName() + " at " + bankFilePath);
        SendNotificationToPlayer(targetPlayer, "Drone Store", "Bank data not found. Cannot complete purchase.", "set:dayz_gui image:icon_error");
        return false;
    }

    DC_DroneBankingData bankData;
    JsonFileLoader<DC_DroneBankingData>.JsonLoadFile(bankFilePath, bankData);

    if (!bankData) {
        Print("[DroneStore] Failed to load bank data for player " + sender.GetName());
        SendNotificationToPlayer(targetPlayer, "Drone Store", "Error loading bank data. Cannot complete purchase.", "set:dayz_gui image:icon_error");
        return false;
    }

    if (bankData.m_OwnedCurrency < kitPrice) {
        Print("[DroneStore] Player " + sender.GetName() + " has insufficient funds.");
        SendNotificationToPlayer(targetPlayer, "Drone Store", "You don't have enough money!", "set:dayz_gui image:icon_warning");
        return false;
    }

    bankData.m_OwnedCurrency -= kitPrice;
    JsonFileLoader<DC_DroneBankingData>.JsonSaveFile(bankFilePath, bankData);

    return true;
}

bool ProcessExpansionBanking(PlayerIdentity sender, PlayerBase targetPlayer, int kitPrice) {
    string playerID = sender.GetId();
    string bankFilePath = "$profile:\\ExpansionMod\\ATM\\" + playerID + ".json";

    if (!FileExist(bankFilePath)) {
        Print("[DroneStore] Bank file not found for player " + sender.GetName() + " at " + bankFilePath);
        SendNotificationToPlayer(targetPlayer, "Drone Store", "Bank data not found. Cannot complete purchase.", "set:dayz_gui image:icon_error");
        return false;
    }

    ExpansionDroneBankingData bankData;
    JsonFileLoader<ExpansionDroneBankingData>.JsonLoadFile(bankFilePath, bankData);

    if (!bankData) {
        Print("[DroneStore] Failed to load bank data for player " + sender.GetName());
        SendNotificationToPlayer(targetPlayer, "Drone Store", "Error loading bank data. Cannot complete purchase.", "set:dayz_gui image:icon_error");
        return false;
    }

    if (bankData.MoneyDeposited < kitPrice) {
        Print("[DroneStore] Player " + sender.GetName() + " has insufficient funds.");
        SendNotificationToPlayer(targetPlayer, "Drone Store", "You don't have enough money!", "set:dayz_gui image:icon_warning");
        return false;
    }

    bankData.MoneyDeposited -= kitPrice;
    JsonFileLoader<ExpansionDroneBankingData>.JsonSaveFile(bankFilePath, bankData);

    return true;
}

bool ProcessKR_Banking(PlayerIdentity sender, PlayerBase targetPlayer, int kitPrice) {
    string steam64ID = sender.GetPlainId();
    string bankFilePath = "$profile:\\KR_BANKING\\PlayerDataBase\\" + steam64ID + ".json";

    if (!FileExist(bankFilePath)) {
        Print("[DroneStore] Bank file not found for player " + sender.GetName() + " at " + bankFilePath);
        SendNotificationToPlayer(targetPlayer, "Drone Store", "Bank data not found. Cannot complete purchase.", "set:dayz_gui image:icon_error");
        return false;
    }

    AdvancedDroneBankingData bankData;
    JsonFileLoader<AdvancedDroneBankingData>.JsonLoadFile(bankFilePath, bankData);

    if (!bankData) {
        Print("[DroneStore] Failed to load bank data for player " + sender.GetName());
        SendNotificationToPlayer(targetPlayer, "Drone Store", "Error loading bank data. Cannot complete purchase.", "set:dayz_gui image:icon_error");
        return false;
    }

    if (bankData.m_OwnedCurrency < kitPrice) {
        Print("[DroneStore] Player " + sender.GetName() + " has insufficient funds.");
        SendNotificationToPlayer(targetPlayer, "Drone Store", "You don't have enough money!", "set:dayz_gui image:icon_warning");
        return false;
    }

    bankData.m_OwnedCurrency -= kitPrice;
    JsonFileLoader<AdvancedDroneBankingData>.JsonSaveFile(bankFilePath, bankData);

    return true;
}

bool ProcessLBmasterBanking(PlayerIdentity sender, PlayerBase targetPlayer, int kitPrice) {
    string steam64ID = sender.GetPlainId();
    string bankFilePath = "$profile:\\LBmaster\\Data\\LBBanking\\Players\\" + steam64ID + ".json";

    if (!FileExist(bankFilePath)) {
        Print("[DroneStore] Bank file not found for player " + sender.GetName() + " at " + bankFilePath);
        SendNotificationToPlayer(targetPlayer, "Drone Store", "Bank data not found. Cannot complete purchase.", "set:dayz_gui image:icon_error");
        return false;
    }

    LBmasterDroneBankingData bankData;
    JsonFileLoader<LBmasterDroneBankingData>.JsonLoadFile(bankFilePath, bankData);

    if (!bankData) {
        Print("[DroneStore] Failed to load bank data for player " + sender.GetName());
        SendNotificationToPlayer(targetPlayer, "Drone Store", "Error loading bank data. Cannot complete purchase.", "set:dayz_gui image:icon_error");
        return false;
    }

    if (bankData.currentMoney < kitPrice) {
        Print("[DroneStore] Player " + sender.GetName() + " has insufficient funds.");
        SendNotificationToPlayer(targetPlayer, "Drone Store", "You don't have enough money!", "set:dayz_gui image:icon_warning");
        return false;
    }

    bankData.currentMoney -= kitPrice;
    JsonFileLoader<LBmasterDroneBankingData>.JsonSaveFile(bankFilePath, bankData);

    return true;
}


    PlayerBase GetPlayerByIdentity(PlayerIdentity identity) {
        if (!identity) {
            Print("MT_Error: Identity is null.");
            return null;
        }

        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);

        for (int i = 0; i < players.Count(); i++) {
            PlayerBase player = PlayerBase.Cast(players.Get(i));

            if (player && player.GetIdentity() && player.GetIdentity().GetId() == identity.GetId()) {
                return player;
            }
        }

        Print("MT_Error: No player found for the given identity.");
        return null;
    }
}

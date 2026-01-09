modded class MissionGameplay {
    ref DroneBuyMenuUI m_DroneBuyMenuUI;
    private bool m_IsDroneBuyMenuUIOpen = false;

    ref KitDataManager kitDataManager;

    void MissionGameplay() {
        if (GetGame().IsClient()) {
            GetRPCManager().AddRPC("psyops_drone", "OpenDroneBuyMenuUI", this, SingeplayerExecutionType.Client);
            GetRPCManager().AddRPC("psyops_drone", "ReceiveKitsFromServer", this, SingeplayerExecutionType.Client);
            GetRPCManager().AddRPC("psyops_drone", "ReceivePlayerBalanceRPC", this, SingeplayerExecutionType.Client);
        }

        kitDataManager = new KitDataManager();
		SendKitsRPC();
       
     //   GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendKitsRPC, 8000, false);
    }

    void SendKitsRPC() {
        GetRPCManager().SendRPC("psyops_drone", "SendKitsToClient", null, true);
    }

    void ReceivePlayerBalanceRPC(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target) {
        if (type == CallType.Client) {
            Param1<int> data;
            if (!ctx.Read(data)) {
                Print("Failed to read data from context");
                return;
            }

            int playerBalance = data.param1;

            if (m_DroneBuyMenuUI && m_IsDroneBuyMenuUIOpen) {
                m_DroneBuyMenuUI.SetPlayerBalance(playerBalance);
            }
        }
    }

    void OpenDroneBuyMenuUI(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target) {
        if (type != CallType.Client) return;
        ToggleDroneBuyMenu();
    }

    void ReceiveKitsFromServer(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target) {
        if (type == CallType.Client) {
            Param1<string> data;
            if (!ctx.Read(data)) {
                Print("Failed to read data from context");
                return;
            }

            string kitsJson = data.param1;

            JsonSerializer js = new JsonSerializer();
            array<ref DeliveryKit> kitsChunk;
            string error;
            bool success = js.ReadFromString(kitsChunk, kitsJson, error);

            if (!success) {
                Print("Failed to deserialize kits data: " + error);
                return;
            }

            foreach (DeliveryKit kit : kitsChunk) {
                kitDataManager.GetKits().Insert(kit);
            }

            Print("Received chunk of kits. Total kits loaded: " + kitDataManager.GetKits().Count());

            if (m_DroneBuyMenuUI && m_IsDroneBuyMenuUIOpen) {
                m_DroneBuyMenuUI.InitializeMenu();
            }
        }
    }

    override void OnUpdate(float timeslice) {
        super.OnUpdate(timeslice);
        Input input = GetGame().GetInput();
        if (input.LocalPress("UAUIBack", false)) {
            if (m_IsDroneBuyMenuUIOpen) {
                CloseDroneBuyMenu();
            }
        }
        if (input.LocalPress("OpenDroneBuyMenuUI", false)) {
            ToggleDroneBuyMenu();
        }
    }

    void ToggleDroneBuyMenu() {
        if (m_DroneBuyMenuUI && m_IsDroneBuyMenuUIOpen) {
            CloseDroneBuyMenu();
        } else if (GetGame().GetUIManager().GetMenu() == NULL) {
            OpenDroneBuyMenu();
        }
    }

    void OpenDroneBuyMenu() {
        LockControls();
        m_DroneBuyMenuUI = DroneBuyMenuUI.Cast(GetUIManager().EnterScriptedMenu(DRONEBUYMENUUI, null));
        if (m_DroneBuyMenuUI) {
            m_DroneBuyMenuUI.SetMenuOpen(true);
            m_DroneBuyMenuUI.SetKitDataManager(kitDataManager);
			//SendKitsRPC();
            if (kitDataManager.GetKits().Count() > 0) {
                m_DroneBuyMenuUI.InitializeMenu();
            } else {
                
                GetRPCManager().SendRPC("psyops_drone", "SendKitsToClient", null, true);
            }
        }
        m_IsDroneBuyMenuUIOpen = true;
    }

    void CloseDroneBuyMenu() {
        GetGame().GetUIManager().HideScriptedMenu(m_DroneBuyMenuUI);
        m_DroneBuyMenuUI = null;
        m_IsDroneBuyMenuUIOpen = false;
        UnlockControls();
    }

    void LockControls() {
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
        GetGame().GetUIManager().ShowUICursor(true);
        GetGame().GetMission().GetHud().Show(false);
    }

    void UnlockControls() {
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetUIManager().ShowUICursor(false);
        GetGame().GetMission().GetHud().Show(true);
    }
}

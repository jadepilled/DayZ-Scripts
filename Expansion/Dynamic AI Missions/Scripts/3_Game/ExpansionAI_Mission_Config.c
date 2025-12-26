/*
Dynamic AI missions for Expansion AI by Touchofdeath Version 2.0

Credits to Sid Debian [76561197991259240] for assisting me with this project! (Moving to server-side mod, and fixing markers)

Credits to Chris (CBD Modding) for assisting me with this project! (Cleaning up bad refs)
*/

static ref AIM_Config g_AIMConfig;

ref AIM_Config GetAIMConfig()
{
	if(!g_AIMConfig)
	{
		g_AIMConfig = g_AIMConfig.LoadConfig();
	}
	
	return g_AIMConfig;
};

class AIM_Config
{
	[NonSerialized()]
	private static const string configRoot = "$profile:/AIMissions/";
	
	[NonSerialized()]
	private static const string configPathSettings = "$profile:/AIMissions/MainConfig.json";

	ref array<ref AIM_SettingsConfig> Settings;
	ref array<ref AIM_MissionsConfig> Missions;
	ref array<ref AIM_LoadoutsConfig> Loadouts;
	ref array<ref AIM_RewardObjects> RewardObjects;
	ref array<ref AIM_LootConfig> Loot;
	
	void AIM_Config()
	{
		Settings = new ref array<ref AIM_SettingsConfig>();
		Missions = new ref array<ref AIM_MissionsConfig>();
		Loadouts = new ref array<ref AIM_LoadoutsConfig>();
		RewardObjects = new ref array<ref AIM_RewardObjects>();
		Loot = new ref array<ref AIM_LootConfig>();
	}
	
	void ~AIM_Config()
	{
		if(this.Settings) this.Settings.Clear();
		this.Settings = NULL;
		if(this.Missions) this.Missions.Clear();
		this.Missions = NULL;
		if(this.Loadouts) this.Loadouts.Clear();
		this.Loadouts = NULL;
		if(this.RewardObjects) this.RewardObjects.Clear();
		this.RewardObjects = NULL;
		if(this.Loot) this.Loot.Clear();
		this.Loot = NULL;
	}
	
	protected void SaveConfigs()
	{
		if(!FileExist(configRoot))
		{
			MakeDirectory(configRoot);
		}
		
		string errorMsgSave = "";
		JsonFileLoader<AIM_Config>.SaveFile(configPathSettings, this, errorMsgSave);
		if(errorMsgSave != "") Print("[AI Missions] JSON SAVE ERROR: " + errorMsgSave);
	};

	void LoadDefaultSettings()
	{
		Settings.Insert(new AIM_SettingsConfig());

		Missions.Insert(new AIM_MissionsConfig("NWAF Runway", "4176.365234 338.773224 10725.653320", 5, 8, 1, 800, 0.5, 1.0, 1.0, true, true));
		Missions.Insert(new AIM_MissionsConfig("Downed Helicopter", "144.312469 319.558411 8073.837402", 3, 5, 1, 400, 0.5, 1.0, 1.0, true, true, false, true, false));
		Missions.Get(Missions.Count() - 1).AddObject("StaticObj_Wreck_Mi8_Crashed", "144.063995 321.147003 8068.359863", "-69.119629 0.000000 -0.000000");
		
		Loadouts.Insert(new AIM_LoadoutsConfig());

		RewardObjects.Insert(new AIM_RewardObjects(1, "SeaChest"));
		RewardObjects.Insert(new AIM_RewardObjects(1, "Barrel_Blue"));
		RewardObjects.Insert(new AIM_RewardObjects(2, "Truck_01_Covered", "", {"Truck_01_Wheel", "Truck_01_Wheel", "Truck_01_WheelDouble", "Truck_01_WheelDouble", "Truck_01_WheelDouble", "Truck_01_WheelDouble", "Truck_01_Door_1_1", "Truck_01_Door_2_1", "Truck_01_Hood", "HeadlightH7", "HeadlightH7"}, {"TruckBattery", "SparkPlug"}));
		RewardObjects.Insert(new AIM_RewardObjects(2, "Offroad_02", "", {"Offroad_02_Wheel", "Offroad_02_Wheel", "Offroad_02_Wheel", "Offroad_02_Wheel", "Offroad_02_Door_1_1", "Offroad_02_Door_1_2", "Offroad_02_Door_2_1", "Offroad_02_Door_2_2", "Offroad_02_Hood", "Offroad_02_Trunk", "HeadlightH7", "HeadlightH7"}, {"Offroad_02_Wheel", "CarBattery", "GlowPlug"}));
		RewardObjects.Insert(new AIM_RewardObjects(2, "OffroadHatchback", "", {"CarRadiator", "HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "HatchbackDoors_Driver", "HatchbackDoors_CoDriver", "HatchbackHood", "HatchbackTrunk", "HeadlightH7", "HeadlightH7"}, {"CarBattery", "SparkPlug"}));

		Loot.Insert(new AIM_LootConfig());

		Loot.Get(0).InsertWeapon("FAL", "Mag_FAL_20Rnd", {"Fal_OeBttsck","ACOGOptic"},{"Mag_FAL_20Rnd","Mag_FAL_20Rnd"});
		Loot.Get(0).InsertWeapon("SVD", "Mag_SVD_10Rnd", {"PSO1Optic"},{"Mag_SVD_10Rnd","Mag_SVD_10Rnd"});

		Loot.Get(0).InsertArmour("PlateCarrierVest");
		Loot.Get(0).InsertArmour("PlateCarrierVest_Black");
		Loot.Get(0).InsertArmour("PlateCarrierVest_Camo");
		Loot.Get(0).InsertArmour("BallisticHelmet_Green");
		Loot.Get(0).InsertArmour("BallisticHelmet_Black");
		Loot.Get(0).InsertArmour("BallisticHelmet_Woodland");

		Loot.Get(0).InsertMisc("BandageDressing");
		Loot.Get(0).InsertMisc("PainkillerTablets");
		Loot.Get(0).InsertMisc("Morphine");
		Loot.Get(0).InsertMisc("SledgeHammer");
		Loot.Get(0).InsertMisc("NailBox");
		Loot.Get(0).InsertMisc("Hatchet");
		Loot.Get(0).InsertMisc("MetalPlate");
		
		SaveConfigs();
	}
	
	static ref AIM_Config LoadConfig() 
	{
		ref AIM_Config settings = new AIM_Config();
		
		if(!FileExist(configRoot))
		{
			MakeDirectory(configRoot);

			settings.LoadDefaultSettings();
		}
		else if(FileExist(configPathSettings))
		{
			string errorMsgLoad = "";
			JsonFileLoader<AIM_Config>.LoadFile(configPathSettings, settings, errorMsgLoad);
			if(errorMsgLoad != "") Print("[AI Missions] JSON LOAD ERROR: " + errorMsgLoad);
			
			bool HasUpdated = false;
			
			if(settings.Settings.Get(0).Config_Version < 1)
			{
				//Old version of config, lets update!
				
				settings.Settings.Get(0).Config_Version = 1;
				
				for(int i=0; i<settings.Missions.Count(); i++)
				{
					settings.Missions.Get(i).Bots_Damage_Done_Multiplier = 1.0;
					settings.Missions.Get(i).Bots_Damage_Taken_Multiplier = 1.0;
					settings.Missions.Get(i).Bots_Drop_Loot = 1;
					settings.Missions.Get(i).Bots_Drop_Weapon = 1;
				}
				
				HasUpdated = true;
			}

			if(settings.Settings.Get(0).Config_Version < 2)
			{
				//Old version of config, lets update!
				
				settings.Settings.Get(0).Config_Version = 2;
				settings.Settings.Get(0).Cleanup_Time_Running = 300;
				settings.Settings.Get(0).Missions_Max_Concurrent = 1;
				settings.Settings.Get(0).Text_MissionTitle = "AI Mission";
				settings.Settings.Get(0).Notification_StartMessage = "Mission '%1' starting! Eliminate all threats to claim the loot!";
				settings.Settings.Get(0).Notification_WinMessage = "Mission '%1' completed! All threats were eliminated!";
				settings.Settings.Get(0).Notification_LoseMessage = "Mission '%1' failed! You ran out of time!";
				settings.Settings.Get(0).Notification_Time = 10;
				settings.Settings.Get(0).Notification_Icon = "Soldier";
				settings.Settings.Get(0).Marker3D_LB_Path = "LBmaster_Groups\\gui\\icons\\player.paa";
				settings.Settings.Get(0).Markers_Icon = "";
				settings.Settings.Get(0).Markers_Show2D = true;
				settings.Settings.Get(0).Markers_Show3D = false;
				settings.Settings.Get(0).Markers_ColourAlpha = 255;
				settings.Settings.Get(0).Markers_ColourRed = 255;
				settings.Settings.Get(0).Markers_ColourGreen = 0;
				settings.Settings.Get(0).Markers_ColourBlue = 0;
				settings.Settings.Get(0).Markers_CircleRadius = 350;
				
				for(int j=0; j<settings.Missions.Count(); j++)
				{
					settings.Missions.Get(j).Bots_Expansion_Loadout = "";
					settings.Missions.Get(j).Allow_Helicopter_Reward = false;
				}
				
				HasUpdated = true;
			}

			if(HasUpdated) settings.SaveConfigs();
		}
		else
		{
			settings.LoadDefaultSettings();
		}
		
		return settings;
	}
}

class AIM_SettingsConfig
{
	float Config_Version;
	int Minimum_Players;
	int Bots_Timeout_Radius;
	int Bots_Timeout_Time;
	int Cleanup_Radius_Startup;
	int Cleanup_Radius_Running;
	int Cleanup_Time_Running;
	int Missions_Max_Concurrent;
	int Mission_Start_Delay;
	int Mission_Starting_Failed_Retry_Delay;
	int Mission_Time_Between;
	int Mission_Time_Limit;
	int Mission_Time_Extension_Limit;
	int Mission_Inner_Radius;
	int Mission_AI_Spawn_Radius;
	int Reward_Loot_Weapons_Minimum;
	int Reward_Loot_Weapons_Maximum;
	float Reward_Loot_Weapons_Multiplier;
	int Reward_Loot_Armour_Minimum;
	int Reward_Loot_Armour_Maximum;
	float Reward_Loot_Armour_Multiplier;
	int Reward_Loot_Misc_Minimum;
	int Reward_Loot_Misc_Maximum;
	float Reward_Loot_Misc_Multiplier;
	// NEW 1.02
	string Text_MissionTitle;
	string Notification_StartMessage;
	string Notification_WinMessage;
	string Notification_LoseMessage;
	int Notification_Time;
	string Notification_Icon;
	string Marker3D_LB_Path;
	string Markers_Icon;
	bool Markers_Show2D;
	bool Markers_Show3D;
	int Markers_ColourAlpha;
	int Markers_ColourRed;
	int Markers_ColourGreen;
	int Markers_ColourBlue;
	int Markers_CircleRadius;
	// END NEW 1.02
	bool Debug_Logging_Enabled;
	bool Debug_Disable_Notifications;
	bool Debug_Disable_Markers;
	bool Debug_Force_Win;
	bool Debug_QuickMode;
	bool Debug_Ignore_Player_Radius;
	
	void AIM_SettingsConfig()
	{
		Config_Version = 2;
		Minimum_Players = 0;
		Bots_Timeout_Radius = 150;
		Bots_Timeout_Time = 300;
		Cleanup_Radius_Startup = 50;
		Cleanup_Radius_Running = 100;
		// NEW 1.02
		Cleanup_Time_Running = 300;
		Missions_Max_Concurrent = 1;
		// END NEW 1.02
		Mission_Start_Delay = 600;
		Mission_Starting_Failed_Retry_Delay = 120;
		Mission_Time_Between = 2700;
		Mission_Time_Limit = 2700;
		Mission_Time_Extension_Limit = 900;
		Mission_Inner_Radius = 500;
		Mission_AI_Spawn_Radius = 1200;
		Reward_Loot_Weapons_Minimum = 1;
		Reward_Loot_Weapons_Maximum = 2;
		Reward_Loot_Weapons_Multiplier = 0.75;
		Reward_Loot_Armour_Minimum = 1;
		Reward_Loot_Armour_Maximum = 2;
		Reward_Loot_Armour_Multiplier = 0.5;
		Reward_Loot_Misc_Minimum = 1;
		Reward_Loot_Misc_Maximum = 4;
		Reward_Loot_Misc_Multiplier = 0.5;
		// NEW 1.02
		Text_MissionTitle = "AI Mission";
		Notification_StartMessage = "Mission '%1' starting! Eliminate all threats to claim the loot!";
		Notification_WinMessage = "Mission '%1' completed! All threats were eliminated!";
		Notification_LoseMessage = "Mission '%1' failed! You ran out of time!";
		Notification_Time = 10;
		Notification_Icon = "Soldier";
		Marker3D_LB_Path = "LBmaster_Groups\\gui\\icons\\player.paa";
		Markers_Icon = "";
		Markers_Show2D = true;
		Markers_Show3D = false;
		Markers_ColourAlpha = 255;
		Markers_ColourRed = 255;
		Markers_ColourGreen = 0;
		Markers_ColourBlue = 0;
		Markers_CircleRadius = 350;
		// END NEW 1.02
		Debug_Logging_Enabled = true;
		Debug_Disable_Notifications = false;
		Debug_Disable_Markers = false;
		Debug_Force_Win = false;
		Debug_QuickMode = false;
		Debug_Ignore_Player_Radius = false;
	}

	void ~AIM_SettingsConfig()
	{
		this.Config_Version = 0;
		this.Minimum_Players = 0;
		this.Bots_Timeout_Radius = 0;
		this.Bots_Timeout_Time = 0;
		this.Cleanup_Radius_Startup = 0;
		this.Cleanup_Radius_Running = 0;
		this.Cleanup_Time_Running = 0;
		this.Missions_Max_Concurrent = 0;
		this.Mission_Start_Delay = 0;
		this.Mission_Starting_Failed_Retry_Delay = 0;
		this.Mission_Time_Between = 0;
		this.Mission_Time_Limit = 0;
		this.Mission_Time_Extension_Limit = 0;
		this.Mission_Inner_Radius = 0;
		this.Mission_AI_Spawn_Radius = 0;
		this.Reward_Loot_Weapons_Minimum = 0;
		this.Reward_Loot_Weapons_Maximum = 0;
		this.Reward_Loot_Weapons_Multiplier = 0;
		this.Reward_Loot_Armour_Minimum = 0;
		this.Reward_Loot_Armour_Maximum = 0;
		this.Reward_Loot_Armour_Multiplier = 0;
		this.Reward_Loot_Misc_Minimum = 0;
		this.Reward_Loot_Misc_Maximum = 0;
		this.Reward_Loot_Misc_Multiplier = 0;
		this.Text_MissionTitle = "";
		this.Notification_StartMessage = "";
		this.Notification_WinMessage = "";
		this.Notification_LoseMessage = "";
		this.Notification_Time = 0;
		this.Notification_Icon = "";
		this.Marker3D_LB_Path = "";
		this.Markers_Icon = "";
		this.Markers_Show2D = false;
		this.Markers_Show3D = false;
		this.Markers_ColourAlpha = 0;
		this.Markers_ColourRed = 0;
		this.Markers_ColourGreen = 0;
		this.Markers_ColourBlue = 0;
		this.Markers_CircleRadius = 0;
		this.Debug_Logging_Enabled = false;
		this.Debug_Disable_Notifications = false;
		this.Debug_Disable_Markers = false;
		this.Debug_Force_Win = false;
		this.Debug_QuickMode = false;
		this.Debug_Ignore_Player_Radius = false;
	}
	
	void DoQuickMode()
	{
		Mission_Time_Between = 15;
		Mission_Time_Limit = 15;
		Mission_Time_Extension_Limit = 5;
		Mission_Start_Delay = 30;
		Debug_Force_Win = true;
	}

	int GetMinPlayers()
	{
		return Minimum_Players;
	}
	
	int GetBotsTimeoutRadius()
	{
		return Bots_Timeout_Radius;
	}
	
	int GetBotsTimeoutTime()
	{
		return Bots_Timeout_Time;
	}

	int GetCleanupRadiusStartup()
	{
		return Cleanup_Radius_Startup;
	}

	int GetCleanupRadiusRunning()
	{
		return Cleanup_Radius_Running;
	}
	
	int GetCleanupTimeRunning()
	{
		return Cleanup_Time_Running;
	}
	
	int GetMaxSimultaneousMissions()
	{
		return Missions_Max_Concurrent;
	}

	int GetStartDelay()
	{
		return Mission_Start_Delay;
	}

	int GetFailStartDelay()
	{
		return Mission_Starting_Failed_Retry_Delay;
	}

	int GetTimeBetweenMissions()
	{
		return Mission_Time_Between;
	}

	int GetTimeLimit()
	{
		return Mission_Time_Limit;
	}

	int GetTimeExtensionMax()
	{
		return Mission_Time_Extension_Limit;
	}

	int GetInnerRadius()
	{
		return Mission_Inner_Radius;
	}

	int GetSpawnRadius()
	{
		return Mission_AI_Spawn_Radius;
	}

	int GetLootWeaponsMin()
	{
		return Reward_Loot_Weapons_Minimum;
	}

	int GetLootWeaponsMax()
	{
		return Reward_Loot_Weapons_Maximum;
	}

	float GetLootWeaponsMultiplier()
	{
		return Reward_Loot_Weapons_Multiplier;
	}

	int GetLootArmourMin()
	{
		return Reward_Loot_Armour_Minimum;
	}

	int GetLootArmourMax()
	{
		return Reward_Loot_Armour_Maximum;
	}

	float GetLootArmourMultiplier()
	{
		return Reward_Loot_Armour_Multiplier;
	}

	int GetLootMiscMin()
	{
		return Reward_Loot_Misc_Minimum;
	}

	int GetLootMiscMax()
	{
		return Reward_Loot_Misc_Maximum;
	}

	float GetLootMiscMultiplier()
	{
		return Reward_Loot_Misc_Multiplier;
	}
	
	string GetMissionsTitle()
	{
		return Text_MissionTitle;
	}
	
	string GetStartMessage()
	{
		return Notification_StartMessage;
	}
	
	string GetWinMessage()
	{
		return Notification_WinMessage;
	}
	
	string GetLoseMessage()
	{
		return Notification_LoseMessage;
	}
	
	int GetNotificationTime()
	{
		return Notification_Time;
	}
	
	string GetNotificationIconString()
	{
		return Notification_Icon;
	}
	
	string GetMarkerFilePath()
	{
		return Marker3D_LB_Path;
	}
	
	string GetMarkerString()
	{
		return Markers_Icon;
	}
	
	bool GetMarker2D()
	{
		return Markers_Show2D;
	}
	
	bool GetMarker3D()
	{
		return Markers_Show3D;
	}
	
	int GetMarkerAlpha()
	{
		return Markers_ColourAlpha;
	}
	
	int GetMarkerRed()
	{
		return Markers_ColourRed;
	}
	
	int GetMarkerGreen()
	{
		return Markers_ColourGreen;
	}
	
	int GetMarkerBlue()
	{
		return Markers_ColourBlue;
	}
	
	int GetMarkerRadius()
	{
		return Markers_CircleRadius;
	}
	
	bool DebugEnabled()
	{
		return Debug_Logging_Enabled;
	}
	
	bool DebugNotificationsDisabled()
	{
		return Debug_Disable_Notifications;
	}
	
	bool DebugMarkersDisabled()
	{
		return Debug_Disable_Markers;
	}
	
	bool DebugForceWin()
	{
		return Debug_Force_Win;
	}
	
	bool DebugQuickmode()
	{
		return Debug_QuickMode;
	}
	
	bool DebugIgnoreRadius()
	{
		return Debug_Ignore_Player_Radius;
	}
}

class AIM_MissionsConfig
{
	string Name;
    vector Position;
	int Bots_Count_Minimum;
	int Bots_Count_Maximum;
	// NEW 1.02
	string Bots_Expansion_Loadout;
	// END NEW 1.02
	int Bots_Loadout_ID;
	int Bots_Range;
	float Bots_Accuracy;
	float Bots_Damage_Done_Multiplier;
	float Bots_Damage_Taken_Multiplier;
	bool Bots_Drop_Loot;
	bool Bots_Drop_Weapon;
	bool Is_NBC_Mission;
	bool Allow_Crate_Reward;
	bool Allow_Vehicle_Reward;
	bool Allow_Helicopter_Reward;
	
	ref array<ref AIM_MissionObjects> Static_Objects;
	
	void AIM_MissionsConfig(string mName, vector mPos, int mBotMinCount, int mBotMaxCount, int mLoadoutID, int mBotRange, float mBotAccuracy, float mBotDmgDone, float mBotDmgTaken, bool mBotDropLoot, bool mBotDropWeapon, bool mIsNBC = false, bool allowcrates = true, bool allowvehicles = true, bool allowhelis = true)
	{
		Name = mName;
		Position = mPos;
		Bots_Count_Minimum = mBotMinCount;
		Bots_Count_Maximum = mBotMaxCount;
		// NEW 1.02
		Bots_Expansion_Loadout = "";
		// END NEW 1.02
		Bots_Loadout_ID = mLoadoutID;
		Bots_Range = mBotRange;
		Bots_Accuracy = mBotAccuracy;
		Bots_Damage_Done_Multiplier = mBotDmgDone;
		Bots_Damage_Taken_Multiplier = mBotDmgTaken;
		Bots_Drop_Loot = mBotDropLoot;
		Bots_Drop_Weapon = mBotDropWeapon;
		Is_NBC_Mission = mIsNBC;
		Allow_Crate_Reward = allowcrates;
		Allow_Vehicle_Reward = allowvehicles;
		Allow_Helicopter_Reward = allowhelis;
		
		Static_Objects = new ref array<ref AIM_MissionObjects>();
	}
	
	void ~AIM_MissionsConfig()
	{
		this.Name = "";
		this.Position = "0 0 0";
		this.Bots_Count_Minimum = 0;
		this.Bots_Count_Maximum = 0;
		this.Bots_Expansion_Loadout = "";
		this.Bots_Loadout_ID = 0;
		this.Bots_Range = 0;
		this.Bots_Accuracy = 0;
		this.Bots_Damage_Done_Multiplier = 0;
		this.Bots_Damage_Taken_Multiplier = 0;
		this.Bots_Drop_Loot = false;
		this.Bots_Drop_Weapon = false;
		this.Is_NBC_Mission = false;
		this.Allow_Crate_Reward = false;
		this.Allow_Vehicle_Reward = false;
		this.Allow_Helicopter_Reward = false;
		
		if(this.Static_Objects) this.Static_Objects.Clear();
		this.Static_Objects = NULL;
	}
	
	void AddObject(string oClass, vector oPos, vector oRot, float oScale = 1.0)
	{
		Static_Objects.Insert(new AIM_MissionObjects(oClass, oPos, oRot, oScale));
	}

	string GetName()
	{
		return Name;
	}
	
	vector GetPosition()
	{
		return Position;
	}
	
	int GetMinAICount()
	{
		return Bots_Count_Minimum;
	}
	
	int GetMaxAICount()
	{
		return Bots_Count_Maximum;
	}
	
	string GetLoadoutName()
	{
		return Bots_Expansion_Loadout;
	}
	
	int GetLoadoutID()
	{
		return Bots_Loadout_ID;
	}
	
	int GetAIRange()
	{
		return Bots_Range;
	}
	
	float GetAIAccuracy()
	{
		return Bots_Accuracy;
	}
	
	float GetAIDamageDoneMultiplier()
	{
		return Bots_Damage_Done_Multiplier;
	}
	
	float GetAIDamageTakenMultiplier()
	{
		return Bots_Damage_Taken_Multiplier;
	}
	
	bool GetAIDropLoot()
	{
		return Bots_Drop_Loot;
	}
	
	bool GetAIDropWeapon()
	{
		return Bots_Drop_Weapon;
	}
	
	bool IsNBCMission()
	{
		return Is_NBC_Mission;
	}
	
	bool AllowCrates()
	{
		return Allow_Crate_Reward;
	}
	
	bool AllowVehicles()
	{
		return Allow_Vehicle_Reward;
	}
	
	bool AllowHelicopters()
	{
		return Allow_Helicopter_Reward;
	}
}

class AIM_MissionObjects
{
	string ClassName;
	float Scale;
	vector Position;
	vector Rotation;
	
	void AIM_MissionObjects(string oClass, vector oPos, vector oRot, float oScale)
	{
		ClassName = oClass;
		Scale = oScale;
		Position = oPos;
		Rotation = oRot;
	}
	
	void ~AIM_MissionObjects()
	{
		this.ClassName = "";
		this.Scale = 0;
		this.Position = "0 0 0";
		this.Rotation = "0 0 0";
	}
	
	string GetClassName()
	{
		return ClassName;
	}
	
	float GetScale()
	{
		return Scale;
	}
	
	vector GetPosition()
	{
		return Position;
	}
	
	vector GetRotation()
	{
		return Rotation;
	}
}

class AIM_LoadoutsConfig
{
	int AI_Loot_Misc_Max;
	int AI_Loot_Special_Max;
	
	ref TStringArray Shirts;
	ref TStringArray Pants;
	ref TStringArray Shoes;
	ref TStringArray BackPacks;
	ref TStringArray Gloves;
	ref TStringArray Belts;
	ref TStringArray NBC_Loadout;
	ref TStringArray AI_Loot_Misc;
	ref TStringArray AI_Loot_Special;
	
	ref array<ref AIM_WeaponsConfig> Weapons;
	ref array<ref AIM_ArmourConfig> Armour;
	ref array<ref AIM_HeadgearConfig> Headgear;
	
	void AIM_LoadoutsConfig()
	{
		AI_Loot_Misc_Max = 2;
		AI_Loot_Special_Max = 1;
		
		Shirts = new TStringArray();
		Pants = new TStringArray();
		Shoes = new TStringArray();
		BackPacks = new TStringArray();
		Gloves = new TStringArray();
		Belts = new TStringArray();
		NBC_Loadout = new TStringArray();
		AI_Loot_Misc = new TStringArray();
		AI_Loot_Special = new TStringArray();

		Shirts.Insert("GorkaEJacket_Autumn");
		Shirts.Insert("GorkaEJacket_Flat");
		Shirts.Insert("GorkaEJacket_PautRev");
		Shirts.Insert("GorkaEJacket_Summer");

		Pants.Insert("GorkaPants_Autumn");
		Pants.Insert("GorkaPants_Flat");
		Pants.Insert("GorkaPants_PautRev");
		Pants.Insert("GorkaPants_Summer");

		Shoes.Insert("TTSKOBoots");
		Shoes.Insert("WorkingBoots_Beige");
		Shoes.Insert("CombatBoots_Beige");
		Shoes.Insert("CombatBoots_Black");
		Shoes.Insert("CombatBoots_Brown");

		BackPacks.Insert("CoyoteBag_Brown");
		BackPacks.Insert("CoyoteBag_Green");
		BackPacks.Insert("HuntingBag");
		BackPacks.Insert("TortillaBag");
		BackPacks.Insert("WaterproofBag_Green");

		Gloves.Insert("WorkingGloves_Beige");
		Gloves.Insert("WorkingGloves_Black");
		Gloves.Insert("NBCGlovesGray");
		Gloves.Insert("OMNOGloves_Brown");

		Belts.Insert("CivilianBelt");
		Belts.Insert("MilitaryBelt");

		NBC_Loadout.Insert("NBCGlovesGray");
		NBC_Loadout.Insert("NBCJacketGray");
		NBC_Loadout.Insert("NBCPantsGray");
		NBC_Loadout.Insert("NBCBootsGray");
		NBC_Loadout.Insert("NBCHoodGray");
		NBC_Loadout.Insert("GasMask");

		AI_Loot_Misc.Insert("SardinesCan");
		AI_Loot_Misc.Insert("SodaCan_Cola");
		AI_Loot_Misc.Insert("SodaCan_Kvass");
		AI_Loot_Misc.Insert("Rice");
		AI_Loot_Misc.Insert("Rope");
		AI_Loot_Misc.Insert("Screwdriver");

		AI_Loot_Special.Insert("NailBox");
		AI_Loot_Special.Insert("Hatchet");
		AI_Loot_Special.Insert("RGD5Grenade");
		
		Weapons = new ref array<ref AIM_WeaponsConfig>();
		Armour = new ref array<ref AIM_ArmourConfig>();
		Headgear = new ref array<ref AIM_HeadgearConfig>();

		Weapons.Insert(new AIM_WeaponsConfig(0, "CZ61", "Mag_CZ61_20Rnd"));
		Weapons.Insert(new AIM_WeaponsConfig(1, "M4A1", "Mag_CMAG_40Rnd", {"M4_RISHndgrd", "M4_MPBttstck", "ACOGOptic"}));
		Weapons.Insert(new AIM_WeaponsConfig(2, "SVD", "Mag_SVD_10Rnd", {"PSO1Optic"}));
		
		Armour.Insert(new AIM_ArmourConfig(0, "PlateCarrierVest_Black"));
		Armour.Insert(new AIM_ArmourConfig(0, "PlateCarrierVest_Green"));
		Armour.Insert(new AIM_ArmourConfig(1, "PlateCarrierVest_Black"));
		Armour.Insert(new AIM_ArmourConfig(1, "PlateCarrierVest_Green"));
		Armour.Insert(new AIM_ArmourConfig(2, "PlateCarrierVest_Camo"));
		
		Headgear.Insert(new AIM_HeadgearConfig(0, "GorkaHelmet", true));
		Headgear.Insert(new AIM_HeadgearConfig(1, "GorkaHelmet", true));
		Headgear.Insert(new AIM_HeadgearConfig(2, "Mich2001Helmet", true));
	}

	void ~AIM_LoadoutsConfig()
	{
		this.AI_Loot_Misc_Max = 0;
		this.AI_Loot_Special_Max = 0;
		if(this.Shirts) this.Shirts.Clear();
		this.Shirts = NULL;
		if(this.Pants) this.Pants.Clear();
		this.Pants = NULL;
		if(this.Shoes) this.Shoes.Clear();
		this.Shoes = NULL;
		if(this.BackPacks) this.BackPacks.Clear();
		this.BackPacks = NULL;
		if(this.Belts) this.Belts.Clear();
		this.Belts = NULL;
		if(this.NBC_Loadout) this.NBC_Loadout.Clear();
		this.NBC_Loadout = NULL;
		if(this.AI_Loot_Misc) this.AI_Loot_Misc.Clear();
		this.AI_Loot_Misc = NULL;
		if(this.AI_Loot_Special) this.AI_Loot_Special.Clear();
		this.AI_Loot_Special = NULL;
		if(this.Weapons) this.Weapons.Clear();
		this.Weapons = NULL;
		if(this.Armour) this.Armour.Clear();
		this.Armour = NULL;
		if(this.Headgear) this.Headgear.Clear();
		this.Headgear = NULL;
	}

	int GetLootMiscMax()
	{
		return AI_Loot_Misc_Max;
	}
	
	int GetLootSpecialMax()
	{
		return AI_Loot_Special_Max;
	}
	
	TStringArray GetShirts()
	{
		if(Shirts != NULL) return Shirts;
		return NULL;
	}
	
	TStringArray GetPants()
	{
		if(Pants != NULL) return Pants;
		return NULL;
	}
	
	TStringArray GetShoes()
	{
		if(Shoes != NULL) return Shoes;
		return NULL;
	}
	
	TStringArray GetBackPacks()
	{
		if(BackPacks != NULL) return BackPacks;
		return NULL;
	}
	
	TStringArray GetGloves()
	{
		if(Gloves != NULL) return Gloves;
		return NULL;
	}
	
	TStringArray GetBelts()
	{
		if(Belts != NULL) return Belts;
		return NULL;
	}
	
	TStringArray GetNBC_Loadout()
	{
		if(NBC_Loadout != NULL) return NBC_Loadout;
		return NULL;
	}
	
	TStringArray GetLootMisc()
	{
		if(AI_Loot_Misc != NULL) return AI_Loot_Misc;
		return NULL;
	}
	
	TStringArray GetLootSpecial()
	{
		if(AI_Loot_Special != NULL) return AI_Loot_Special;
		return NULL;
	}
}

class AIM_WeaponsConfig
{
	int Loadout_ID;
	string Weapon_ClassName;
	string Magazine_ClassName;
	ref TStringArray Attach;
	
	void AIM_WeaponsConfig(int mLoadoutID, string mClassName, string mMagClassName, TStringArray mAttachments = NULL)
	{
		Loadout_ID = mLoadoutID;
		Weapon_ClassName = mClassName;
		Magazine_ClassName = mMagClassName;
		
		Attach = new TStringArray();

		if(mAttachments != NULL) Attach = mAttachments;
	}
	
	void ~AIM_WeaponsConfig()
	{
		this.Loadout_ID = 0;
		this.Weapon_ClassName = "";
		this.Magazine_ClassName = "";
		if(this.Attach) this.Attach.Clear();
		this.Attach = NULL;
	}
	
	int GetLoadoutID()
	{
		return Loadout_ID;
	}
	
	string GetClassName()
	{
		return Weapon_ClassName;
	}

	string GetMagClassName()
	{
		return Magazine_ClassName;
	}
	
	TStringArray GetAttachments()
	{
		if(Attach != NULL) return Attach;
		return NULL;
	}
}

class AIM_ArmourConfig
{
	int Loadout_ID;
	string Armour_ClassName;
	ref TStringArray Attach;
	ref TStringArray Contents;
	
	void AIM_ArmourConfig(int mLoadoutID, string mClassName, TStringArray mAttachments = NULL, TStringArray mContents = NULL)
	{
		Loadout_ID = mLoadoutID;
		Armour_ClassName = mClassName;

		Attach = new TStringArray();
		Contents = new TStringArray();

		if(mAttachments != NULL) Attach = mAttachments;
		if(mContents != NULL) Contents = mContents;
	}
	
	void ~AIM_ArmourConfig()
	{
		this.Loadout_ID = 0;
		this.Armour_ClassName = "";
		if(this.Attach) this.Attach.Clear();
		this.Attach = NULL;
		if(this.Contents) this.Contents.Clear();
		this.Contents = NULL;
	}
	
	int GetLoadoutID()
	{
		return Loadout_ID;
	}
	
	string GetClassName()
	{
		return Armour_ClassName;
	}
	
	TStringArray GetAttachments()
	{
		if(Attach != NULL) return Attach;
		return NULL;
	}
	
	TStringArray GetContents()
	{
		if(Contents != NULL) return Contents;
		return NULL;
	}
}

class AIM_HeadgearConfig
{
	int Loadout_ID;
	string Helmet_ClassName;
	bool Give_NVG;
	
	void AIM_HeadgearConfig(int mLoadoutID, string mClassName, bool mGiveNVG)
	{
		Loadout_ID = mLoadoutID;
		Helmet_ClassName = mClassName;
		Give_NVG = mGiveNVG;
	}
	
	void ~AIM_HeadgearConfig()
	{
		this.Loadout_ID = 0;
		this.Helmet_ClassName = "";
		Give_NVG = false;
	}
	
	int GetLoadoutID()
	{
		return Loadout_ID;
	}
	
	string GetClassName()
	{
		return Helmet_ClassName;
	}
	
	bool GiveNVG()
	{
		return Give_NVG;
	}
}

class AIM_RewardObjects
{
	int Reward_Type;
	string ClassName;
	string Replace_ClassName;
	ref TStringArray Loot_Initial;
	ref TStringArray Loot_Win;

	void AIM_RewardObjects(int rType, string rClass, string rNewClass = "", TStringArray rLootInit = NULL, TStringArray rLootWin = NULL)
	{
		Reward_Type = rType;
		ClassName = rClass;
		Replace_ClassName = rNewClass;

		Loot_Initial = new TStringArray();
		Loot_Win = new TStringArray();

		if(rLootInit != NULL) Loot_Initial = rLootInit;
		if(rLootInit != NULL) Loot_Win = rLootWin;
	}
	
	void ~AIM_RewardObjects()
	{
		this.Reward_Type = 0;
		this.ClassName = "";
		this.Replace_ClassName = "";
		if(this.Loot_Initial) this.Loot_Initial.Clear();
		this.Loot_Initial = NULL;
		if(this.Loot_Win) this.Loot_Win.Clear();
		this.Loot_Win = NULL;
	}
	
	int GetRewardType()
	{
		return Reward_Type;
	}
	
	string GetClassname()
	{
		return ClassName;
	}
	
	string GetNewClassname()
	{
		return Replace_ClassName;
	}
	
	TStringArray GetLootInit()
	{
		return Loot_Initial;
	}
	
	TStringArray GetLootWin()
	{
		return Loot_Win;
	}
}

class AIM_LootConfig
{
	ref array<ref AIM_LootConfig_Weapon> Weapons;
	ref array<ref AIM_LootConfig_Child> Armour;
	ref array<ref AIM_LootConfig_Child> Misc;
	
	void AIM_LootConfig()
	{
		Weapons = new ref array<ref AIM_LootConfig_Weapon>();
		Armour = new ref array<ref AIM_LootConfig_Child>();
		Misc = new ref array<ref AIM_LootConfig_Child>();
	}
	
	void ~AIM_LootConfig()
	{
		if(this.Weapons) this.Weapons.Clear();
		this.Weapons = NULL;
		if(this.Armour) this.Armour.Clear();
		this.Armour = NULL;
		if(this.Misc) this.Misc.Clear();
		this.Misc = NULL;
	}
	
	void InsertWeapon(string sClass, string sMag = "", TStringArray sAtt = NULL, TStringArray sLoot = NULL)
	{
		Weapons.Insert(new AIM_LootConfig_Weapon(sClass, sMag, sAtt, sLoot));
	}
	
	void InsertArmour(string sClass, TStringArray sAtt = NULL, TStringArray sLoot = NULL)
	{
		Armour.Insert(new AIM_LootConfig_Child(sClass, sAtt, sLoot));
	}
	
	void InsertMisc(string sClass, TStringArray sAtt = NULL, TStringArray sLoot = NULL)
	{
		Misc.Insert(new AIM_LootConfig_Child(sClass, sAtt, sLoot));
	}
}

class AIM_LootConfig_Weapon
{
	string ClassName;
	string Magazine_ClassName;
	ref TStringArray Attach;
	ref TStringArray Loot;
	
	void AIM_LootConfig_Weapon(string sClass, string sMag, TStringArray sAtt, TStringArray sLoot)
	{
		ClassName = sClass;
		Magazine_ClassName = sMag;
		
		Attach = new TStringArray();
		Loot = new TStringArray();

		if(sAtt != NULL) Attach = sAtt;
		if(sLoot != NULL) Loot = sLoot;
	}
	
	void ~AIM_LootConfig_Weapon()
	{
		this.ClassName = "";
		this.Magazine_ClassName = "";
		if(this.Attach) this.Attach.Clear();
		this.Attach = NULL;
		if(this.Loot) this.Loot.Clear();
		this.Loot = NULL;
	}
	
	string GetClassname()
	{
		return ClassName;
	}
	
	string GetMagClassName()
	{
		return Magazine_ClassName;
	}
	
	TStringArray GetAttachments()
	{
		return Attach;
	}
	
	TStringArray GetLoot()
	{
		return Loot;
	}
}

class AIM_LootConfig_Child
{
	string ClassName;
	ref TStringArray Attach;
	ref TStringArray Contents;
	
	void AIM_LootConfig_Child(string sClass, TStringArray sAtt, TStringArray sLoot)
	{
		ClassName = sClass;
		
		Attach = new TStringArray();
		Contents = new TStringArray();

		if(sAtt != NULL) Attach = sAtt;
		if(sLoot != NULL) Contents = sLoot;
	}
	
	void ~AIM_LootConfig_Child()
	{
		this.ClassName = "";
		if(this.Attach) this.Attach.Clear();
		this.Attach = NULL;
		if(this.Contents) this.Contents.Clear();
		this.Contents = NULL;
	}
	
	string GetClassname()
	{
		return ClassName;
	}
	
	TStringArray GetAttachments()
	{
		return Attach;
	}
	
	TStringArray GetLoot()
	{
		return Contents;
	}	
}
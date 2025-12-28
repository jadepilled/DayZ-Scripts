class DAMEventDirector
{
protected ref DAMOverhaulConfig m_Config;
protected ref array<ref DAMMissionBase> m_ActiveMissions = new array<ref DAMMissionBase>();

void DAMEventDirector()
{
m_Config = new DAMOverhaulConfig();
RegisterDefaultTiers();
RegisterDefaultWeights();
RegisterDefaultLoot();
RegisterDefaultMissions();
}

void RegisterDefaultTiers()
{
m_Config.RegisterTier(new DAMTierSettings("TierOne", 8, 1.0, 1.0, 1.0, 1));
m_Config.RegisterTier(new DAMTierSettings("TierTwo", 12, 1.5, 1.25, 0.85, 2));
m_Config.RegisterTier(new DAMTierSettings("TierThree", 18, 2.0, 1.5, 0.65, 3));
m_Config.RegisterTier(new DAMTierSettings("TierFour", 22, 2.25, 1.75, 0.55, 4));
m_Config.RegisterTier(new DAMTierSettings("TierFive", 28, 2.5, 2.0, 0.45, 5));
m_Config.RegisterTier(new DAMTierSettings("TierSix", 34, 3.0, 2.25, 0.35, 6));
}

void RegisterDefaultWeights()
{
m_Config.SetEventWeight("Escort", 1.0);
m_Config.SetEventWeight("VehicleDefense", 0.9);
m_Config.SetEventWeight("TerritoryWar", 0.75);
}

void RegisterDefaultLoot()
{
ref array<string> tierOneLoot = {"BandageDressing", "Mag_UMP_25Rnd", "Canteen"};
ref array<string> tierTwoLoot = {"AK101", "Mag_AK101_30Rnd", "SmershBag_Green"};
ref array<string> tierThreeLoot = {"SVD", "Mag_SVD_10Rnd", "GhillieHood_Woodland"};
ref array<string> tierFourLoot = {"NVGoggles", "AKM", "Mag_AKM_Drum75Rnd"};
ref array<string> tierFiveLoot = {"M4A1_Green", "Mag_STANAGCoupled_30Rnd", "PlateCarrierVest"};
ref array<string> tierSixLoot = {"FAL", "Mag_FAL_20Rnd", "SVD"};

m_Config.RegisterLootTier(new DAMLootTierItems(1, tierOneLoot));
m_Config.RegisterLootTier(new DAMLootTierItems(2, tierTwoLoot));
m_Config.RegisterLootTier(new DAMLootTierItems(3, tierThreeLoot));
m_Config.RegisterLootTier(new DAMLootTierItems(4, tierFourLoot));
m_Config.RegisterLootTier(new DAMLootTierItems(5, tierFiveLoot));
m_Config.RegisterLootTier(new DAMLootTierItems(6, tierSixLoot));
}

void RegisterDefaultMissions()
{
ref DAMMissionConfig escortConfig = new DAMMissionConfig();
escortConfig.MissionId = "escort_default";
escortConfig.MissionType = "Escort";
escortConfig.TierName = "TierTwo";
escortConfig.IsEscortMission = true;
escortConfig.EscortPath.Insert("5000 0 5000");
escortConfig.EscortPath.Insert("5050 0 5050");
escortConfig.EscortPath.Insert("5100 0 5100");
escortConfig.Messages = new DAMMissionMessages("Village Escort", "Escort mission started", "Escort complete", "Escort failed", 300.0);
m_Config.RegisterMissionConfig(escortConfig);

ref DAMMissionConfig vehicleConfig = new DAMMissionConfig();
vehicleConfig.MissionId = "vehicle_default";
vehicleConfig.MissionType = "VehicleDefense";
vehicleConfig.TierName = "TierThree";
vehicleConfig.Origin = "6000 0 6000";
vehicleConfig.VehicleSettings = new DAMVehicleRewardSettings(true, 45.0);
vehicleConfig.Messages = new DAMMissionMessages("Vehicle Defense", "Defend the vehicle!", "Vehicle secured", "Vehicle lost", 250.0);
m_Config.RegisterMissionConfig(vehicleConfig);

ref DAMMissionConfig warConfig = new DAMMissionConfig();
warConfig.MissionId = "territory_default";
warConfig.MissionType = "TerritoryWar";
warConfig.TierName = "TierFour";
warConfig.IsTerritoryWarMission = true;
warConfig.FactionAClassName = "eAIFactionLunatics";
warConfig.FactionBClassName = "eAIFactionInfected";
warConfig.Origin = "7000 0 7000";
warConfig.Messages = new DAMMissionMessages("Territory War", "Territory War initiated", "Territory secured", "Territory lost", 400.0);
m_Config.RegisterMissionConfig(warConfig);
}

DAMMissionBase CreateEscortMission(array<vector> path, DAMTierSettings tier, DAMMissionMessages messages)
{
vector origin = path.Count() > 0 ? path[0] : ChooseRandomLocation(messages.MarkerRadius);
ref DAMEscortMission mission = new DAMEscortMission("escort" + Math.RandomInt(0, int.MAX), tier, origin, messages, path, m_Config);
return mission;
}

DAMMissionBase CreateVehicleDefenseMission(vector origin, DAMTierSettings tier, DAMMissionMessages messages, DAMVehicleRewardSettings vehicleSettings, float timeLimit)
{
ref DAMVehicleDefenseMission mission = new DAMVehicleDefenseMission("vehicle_defense" + Math.RandomInt(0, int.MAX), tier, origin, messages, vehicleSettings, timeLimit, m_Config);
return mission;
}

DAMMissionBase CreateTerritoryWarMission(vector origin, DAMTierSettings tier, DAMMissionMessages messages, string factionA = "", string factionB = "")
{
ref DAMTerritoryWarMission mission = new DAMTerritoryWarMission("territory_war" + Math.RandomInt(0, int.MAX), tier, origin, messages, m_Config, factionA, factionB);
return mission;
}

void StartMission(ref DAMMissionBase mission)
{
if (!mission)
{
return;
}

mission.StartMission();
m_ActiveMissions.Insert(mission);
}

void Tick(float deltaTime)
{
for (int i = m_ActiveMissions.Count() - 1; i >= 0; i--)
{
DAMMissionBase mission = m_ActiveMissions[i];
if (mission && mission.IsActive())
{
mission.Tick(deltaTime);
continue;
}

m_ActiveMissions.Remove(i);
}
}

float GetWeightFor(string missionType)
{
return m_Config.GetEventWeight(missionType);
}

vector ChooseRandomLocation(float radius)
{
vector center = "0 0 0";
float randomX = Math.RandomFloatInclusive(-radius, radius);
float randomZ = Math.RandomFloatInclusive(-radius, radius);
float y = GetGame().SurfaceY(randomX, randomZ);
return Vector(randomX, y, randomZ);
}

ref DAMMissionBase CreateMissionFromConfig(ref DAMMissionConfig missionConfig)
{
if (!missionConfig || !missionConfig.IsValid())
{
return null;
}

DAMTierSettings tier;
if (!m_Config.TryGetTier(missionConfig.TierName, tier))
{
Print(string.Format("[DAM] Missing tier %1 for mission %2", missionConfig.TierName, missionConfig.MissionId));
return null;
}

vector origin = missionConfig.Origin;
if (missionConfig.UseRandomOrigin)
{
origin = ChooseRandomLocation(missionConfig.RandomRadius);
}

switch (missionConfig.MissionType)
{
case "Escort":
return CreateEscortMission(missionConfig.EscortPath, tier, missionConfig.Messages);
case "VehicleDefense":
return CreateVehicleDefenseMission(origin, tier, missionConfig.Messages, missionConfig.VehicleSettings, missionConfig.TimeLimit);
case "TerritoryWar":
return CreateTerritoryWarMission(origin, tier, missionConfig.Messages, missionConfig.FactionAClassName, missionConfig.FactionBClassName);
default:
Print(string.Format("[DAM] Unknown mission type %1 for mission %2", missionConfig.MissionType, missionConfig.MissionId));
break;
}

return null;
}
}

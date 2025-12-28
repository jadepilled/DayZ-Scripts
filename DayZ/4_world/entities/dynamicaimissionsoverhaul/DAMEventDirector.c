class DAMEventDirector
{
protected ref DAMOverhaulConfig m_Config;
protected ref array<ref DAMMissionBase> m_ActiveMissions = new array<ref DAMMissionBase>();

void DAMEventDirector()
{
m_Config = new DAMOverhaulConfig();
RegisterDefaultTiers();
RegisterDefaultWeights();
}

void RegisterDefaultTiers()
{
m_Config.RegisterTier(new DAMTierSettings("TierOne", 8, 1.0, 1.0, 1.0));
m_Config.RegisterTier(new DAMTierSettings("TierTwo", 12, 1.5, 1.25, 0.85));
m_Config.RegisterTier(new DAMTierSettings("TierThree", 18, 2.0, 1.5, 0.65));
}

void RegisterDefaultWeights()
{
m_Config.SetEventWeight("Escort", 1.0);
m_Config.SetEventWeight("VehicleDefense", 0.9);
m_Config.SetEventWeight("TerritoryWar", 0.75);
}

DAMMissionBase CreateEscortMission(array<vector> path, DAMTierSettings tier, DAMMissionMessages messages)
{
vector origin = path.Count() > 0 ? path[0] : ChooseRandomLocation(messages.MarkerRadius);
ref DAMEscortMission mission = new DAMEscortMission("escort" + Math.RandomInt(0, int.Max), tier, origin, messages, path);
return mission;
}

DAMMissionBase CreateVehicleDefenseMission(vector origin, DAMTierSettings tier, DAMMissionMessages messages, DAMVehicleRewardSettings vehicleSettings, float timeLimit)
{
ref DAMVehicleDefenseMission mission = new DAMVehicleDefenseMission("vehicle_defense" + Math.RandomInt(0, int.Max), tier, origin, messages, vehicleSettings, timeLimit);
return mission;
}

DAMMissionBase CreateTerritoryWarMission(vector origin, DAMTierSettings tier, DAMMissionMessages messages)
{
ref DAMTerritoryWarMission mission = new DAMTerritoryWarMission();
mission.DAMMissionBase("territory_war" + Math.RandomInt(0, int.Max), tier, origin, messages, 0);
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
}

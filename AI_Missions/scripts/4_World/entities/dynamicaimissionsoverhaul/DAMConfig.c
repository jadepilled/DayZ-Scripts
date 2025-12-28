class DAMTierSettings
{
string Name;
int EnemyCount;
float EnemySkillMultiplier;
float LootQualityMultiplier;
float FrequencyWeight;
int LootTier;

void DAMTierSettings(string name = "", int enemyCount = 0, float skillMultiplier = 1.0, float lootMultiplier = 1.0, float frequencyWeight = 1.0, int lootTier = 1)
{
Name = name;
EnemyCount = enemyCount;
EnemySkillMultiplier = skillMultiplier;
LootQualityMultiplier = lootMultiplier;
FrequencyWeight = frequencyWeight;
LootTier = lootTier;
}

bool IsValid()
{
return Name != "" && EnemyCount > 0 && EnemySkillMultiplier > 0 && LootQualityMultiplier > 0 && FrequencyWeight > 0 && LootTier > 0;
}
}

class DAMLootTierItems
{
int TierNumber;
ref array<string> LootItems;

void DAMLootTierItems(int tierNumber = 1, array<string> items = null)
{
TierNumber = tierNumber;
if (items)
{
LootItems = items;
}
else
{
LootItems = new array<string>;
}
}

bool IsValid()
{
return TierNumber > 0 && LootItems && LootItems.Count() > 0;
}
}

class DAMMissionMessages
{
string Title;
string StartMessage;
string SuccessMessage;
string FailureMessage;
float MarkerRadius;

void DAMMissionMessages(string title = "", string startMsg = "", string successMsg = "", string failureMsg = "", float markerRadius = 300.0)
{
Title = title;
StartMessage = startMsg;
SuccessMessage = successMsg;
FailureMessage = failureMsg;
MarkerRadius = markerRadius;
}
}

class DAMVehicleRewardSettings
{
bool IncludeVehicleAsReward;
float DespawnDelayOnFailure;

void DAMVehicleRewardSettings(bool includeVehicle = false, float despawnDelay = 30.0)
{
IncludeVehicleAsReward = includeVehicle;
DespawnDelayOnFailure = despawnDelay;
}
}

class DAMMissionConfig
{
string MissionId;
string MissionType;
string TierName;
vector Origin;
float TimeLimit;
ref DAMMissionMessages Messages;
ref DAMVehicleRewardSettings VehicleSettings;
bool UseRandomOrigin;
float RandomRadius;

bool IsEscortMission;
ref array<vector> EscortPath;

bool IsTerritoryWarMission;
string FactionAClassName;
string FactionBClassName;

void DAMMissionConfig()
{
Messages = new DAMMissionMessages();
VehicleSettings = new DAMVehicleRewardSettings();
EscortPath = new array<vector>();
RandomRadius = 300.0;
}

bool IsValid()
{
return MissionId != "" && MissionType != "" && TierName != "" && Messages != null;
}
}

class DAMOverhaulConfig
{
ref map<string, ref DAMTierSettings> m_Tiers = new map<string, ref DAMTierSettings>();
ref map<string, float> m_EventWeights = new map<string, float>();
ref map<int, ref array<string>> m_LootByTier = new map<int, ref array<string>>();
ref array<ref DAMMissionConfig> m_MissionConfigs = new array<ref DAMMissionConfig>();
float m_GlobalEventFrequencyMultiplier = 1.0;

void RegisterTier(ref DAMTierSettings tier)
{
if (!tier || !tier.IsValid())
{
return;
}

m_Tiers.Set(tier.Name, tier);
}

bool TryGetTier(string name, out DAMTierSettings tier)
{
return m_Tiers.Find(name, tier);
}

void SetEventWeight(string eventType, float weight)
{
if (weight <= 0)
{
return;
}

m_EventWeights.Set(eventType, weight);
}

float GetEventWeight(string eventType)
{
float weight;
if (m_EventWeights.Find(eventType, weight))
{
return weight * m_GlobalEventFrequencyMultiplier;
}

return m_GlobalEventFrequencyMultiplier;
}

void RegisterLootTier(ref DAMLootTierItems lootTier)
{
if (!lootTier || !lootTier.IsValid())
{
return;
}

m_LootByTier.Set(lootTier.TierNumber, lootTier.LootItems);
}

bool TryGetLootForTier(int tierNumber, out array<string> lootItems)
{
return m_LootByTier.Find(tierNumber, lootItems);
}

void RegisterMissionConfig(ref DAMMissionConfig config)
{
if (!config || !config.IsValid())
{
return;
}

m_MissionConfigs.Insert(config);
}

ref array<ref DAMMissionConfig> GetMissionConfigs()
{
return m_MissionConfigs;
}
}

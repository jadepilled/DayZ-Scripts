class DAMTierSettings
{
string Name;
int EnemyCount;
float EnemySkillMultiplier;
float LootQualityMultiplier;
float FrequencyWeight;

void DAMTierSettings(string name = "", int enemyCount = 0, float skillMultiplier = 1.0, float lootMultiplier = 1.0, float frequencyWeight = 1.0)
{
Name = name;
EnemyCount = enemyCount;
EnemySkillMultiplier = skillMultiplier;
LootQualityMultiplier = lootMultiplier;
FrequencyWeight = frequencyWeight;
}

bool IsValid()
{
return Name != "" && EnemyCount > 0 && EnemySkillMultiplier > 0 && LootQualityMultiplier > 0 && FrequencyWeight > 0;
}
}

class DAMMissionMessages
{
string StartMessage;
string SuccessMessage;
string FailureMessage;
float MarkerRadius;

void DAMMissionMessages(string startMsg = "", string successMsg = "", string failureMsg = "", float markerRadius = 300.0)
{
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

class DAMOverhaulConfig
{
ref map<string, ref DAMTierSettings> m_Tiers = new map<string, ref DAMTierSettings>();
ref map<string, float> m_EventWeights = new map<string, float>();
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
}

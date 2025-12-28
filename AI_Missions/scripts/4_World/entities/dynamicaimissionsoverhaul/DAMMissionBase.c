class DAMMissionBase
{
protected string m_MissionId;
protected ref DAMMissionMessages m_Messages;
protected ref DAMTierSettings m_Tier;
protected ref DAMOverhaulConfig m_Config;
protected vector m_Location;
protected bool m_IsActive;
protected float m_TimeLimit;
protected float m_Elapsed;
protected float m_CleanupDelay = 60.0;

void DAMMissionBase(string missionId, DAMTierSettings tier, vector origin, DAMMissionMessages messages, float timeLimit = 1800, DAMOverhaulConfig config = null)
{
m_MissionId = missionId;
m_Tier = tier;
m_Location = origin;
m_Messages = messages;
m_TimeLimit = timeLimit;
m_Elapsed = 0;
m_Config = config;
}

void StartMission()
{
if (m_IsActive)
{
return;
}

m_IsActive = true;
m_Elapsed = 0;
OnStart();
NotifyPlayers(m_Messages.StartMessage);
CreateMarker();
}

void Tick(float deltaTime)
{
if (!m_IsActive)
{
return;
}

m_Elapsed += deltaTime;
OnTick(deltaTime);

if (!m_IsActive)
{
return;
}

OnThink(deltaTime);

if (!m_IsActive)
{
return;
}

if (m_TimeLimit > 0 && m_Elapsed >= m_TimeLimit)
{
FailMission("Time limit reached");
}
}

void CompleteMission()
{
if (!m_IsActive)
{
return;
}

m_IsActive = false;
OnComplete();
SpawnLootRewards();
NotifyPlayers(m_Messages.SuccessMessage);
CleanupMarker();
}

void FailMission(string reason = "")
{
if (!m_IsActive)
{
return;
}

m_IsActive = false;
OnFail(reason);
string message = m_Messages.FailureMessage;
if (reason != "")
{
message = message + " (" + reason + ")";
}

NotifyPlayers(message);
CleanupMarker();
}

bool IsActive()
{
return m_IsActive;
}

protected void NotifyPlayers(string message)
{
if (message == "")
{
return;
}

array<Man> players = new array<Man>();
GetGame().GetPlayers(players);

for (int i = 0; i < players.Count(); i++)
{
PlayerBase player;
Class.CastTo(player, players.Get(i));
if (player)
{
string composedMessage = message;
if (m_Messages && m_Messages.Title != "")
{
composedMessage = m_Messages.Title + " - " + message;
}

player.MessageStatus(composedMessage);
}
}
}

protected void CreateMarker()
{
if (m_Messages && m_Messages.MarkerRadius > 0)
{
string markerLog = string.Format("[DAM] Created mission marker for %1 at %2 with radius %3", m_MissionId, m_Location.ToString(), m_Messages.MarkerRadius);
Print(markerLog);
}
}

protected void CleanupMarker()
{
Print(string.Format("[DAM] Removed mission marker for %1", m_MissionId));
}

protected vector FindRandomizedPosition(vector origin, float radius)
{
float angle = Math.RandomFloatInclusive(0, Math.PI2);
float distance = Math.RandomFloatInclusive(0, radius);
float x = origin[0] + Math.Cos(angle) * distance;
float z = origin[2] + Math.Sin(angle) * distance;
float y = GetGame().SurfaceY(x, z);
return Vector(x, y, z);
}

protected void OnStart()
{
}

protected void OnTick(float deltaTime)
{
}

protected void OnThink(float deltaTime)
{
}

protected void OnComplete()
{
}

protected void OnFail(string reason)
{
}

protected void QueueCleanup(Object obj, int delayMs = 60000)
{
if (!obj)
{
return;
}

GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(GetGame().ObjectDelete, delayMs, false, obj);
}

protected eAIBase SpawnAIAt(vector position, eAIFaction faction = null, string className = "eAI_SurvivorM_Mirek")
{
Object obj = GetGame().CreateObject(className, position, true, false, true);
eAIBase ai;
if (Class.CastTo(ai, obj))
{
eAIGroup.GetGroupByLeader(ai, true, faction);
}

return ai;
}

protected void SpawnLootRewards()
{
if (!m_Config || !m_Tier)
{
return;
}

array<string> lootItems;
if (!m_Config.TryGetLootForTier(m_Tier.LootTier, lootItems) || !lootItems || lootItems.Count() == 0)
{
Print(string.Format("[DAM] No loot configured for tier %1", m_Tier.LootTier));
return;
}

int rewardCount = Math.Clamp(m_Tier.LootTier, 1, 6);
for (int i = 0; i < rewardCount; i++)
{
int index = Math.RandomIntInclusive(0, lootItems.Count() - 1);
string rewardClass = lootItems[index];
vector spawnPos = FindRandomizedPosition(m_Location, 2.0);

Object rewardObj = GetGame().CreateObject(rewardClass, spawnPos, false, true, true);
ItemBase rewardItem;
if (Class.CastTo(rewardItem, rewardObj))
{
rewardItem.SetQuantityMax();
}

Print(string.Format("[DAM] Spawned %1 loot reward %2 at %3", m_MissionId, rewardClass, spawnPos.ToString()));
}
}
}

class DAMEscortMission : DAMMissionBase
{
protected ref array<vector> m_PathPoints = new array<vector>();
protected int m_CurrentWaypointIndex = -1;
protected Object m_EscortTarget;
protected float m_AdvanceRadius = 40.0;
protected float m_SpawnRadius = 125.0;
protected float m_PlayerCheckInterval = 2.0;
protected float m_PlayerCheckTimer;

void DAMEscortMission(string missionId, DAMTierSettings tier, vector origin, DAMMissionMessages messages, array<vector> path, DAMOverhaulConfig config = null)
{
super.DAMMissionBase(missionId, tier, origin, messages, 0, config);

if (path)
{
m_PathPoints.Copy(path);
}
}

void ActivateFromPlayer(PlayerBase player)
{
if (!IsActive())
{
m_Location = player.GetPosition();
StartMission();
}
}

override protected void OnStart()
{
SpawnEscortTarget();
m_CurrentWaypointIndex = 0;
Print(string.Format("[DAM] Escort mission %1 started with %2 path points", m_MissionId, m_PathPoints.Count()));
}

override protected void OnTick(float deltaTime)
{
if (!IsTargetAlive())
{
FailMission("Escort target lost");
return;
}

HandleWaypointProgress(deltaTime);
}

override protected void OnThink(float deltaTime)
{
if (!IsTargetAlive())
{
FailMission("Escort target killed");
return;
}

if (m_CurrentWaypointIndex < 0 || m_CurrentWaypointIndex >= m_PathPoints.Count())
{
return;
}

m_PlayerCheckTimer -= deltaTime;
if (m_PlayerCheckTimer > 0)
{
return;
}

m_PlayerCheckTimer = m_PlayerCheckInterval;
float distance = GetClosestPlayerDistance(m_PathPoints[m_CurrentWaypointIndex]);
if (distance <= m_AdvanceRadius)
{
AdvanceEscortToWaypoint(m_PathPoints[m_CurrentWaypointIndex]);
SpawnEscortEncounter(m_PathPoints[m_CurrentWaypointIndex]);
m_CurrentWaypointIndex++;
}
}

override protected void OnComplete()
{
Print(string.Format("[DAM] Escort mission %1 reached final waypoint", m_MissionId));
}

override protected void OnFail(string reason)
{
Print(string.Format("[DAM] Escort mission %1 failed: %2", m_MissionId, reason));
}

protected void SpawnEscortTarget()
{
vector spawnPos = m_Location;
if (m_PathPoints.Count() > 0)
{
spawnPos = m_PathPoints[0];
}

m_EscortTarget = SpawnAIAt(spawnPos, null, "eAI_SurvivorM_Mirek");

if (m_EscortTarget)
{
Print(string.Format("[DAM] Spawned escort target for %1 at %2", m_MissionId, spawnPos.ToString()));
}
}

protected void HandleWaypointProgress(float deltaTime)
{
if (m_CurrentWaypointIndex < 0 || m_CurrentWaypointIndex >= m_PathPoints.Count())
{
CompleteMission();
return;
}
}

protected void SpawnEscortEncounter(vector waypoint)
{
int aiCount = m_Tier.EnemyCount;
for (int i = 0; i < aiCount; i++)
{
vector spawnPos = FindRandomizedPosition(waypoint, m_SpawnRadius);
SpawnPresetAI(spawnPos);
}
}

protected void AdvanceEscortToWaypoint(vector waypoint)
{
if (!m_EscortTarget)
{
return;
}

m_EscortTarget.SetPosition(waypoint);
}

protected float GetClosestPlayerDistance(vector toPoint)
{
float closest = Math.POSITIVE_INFINITY;
array<Man> players = new array<Man>();
GetGame().GetPlayers(players);

for (int i = 0; i < players.Count(); i++)
{
float distance = vector.Distance(players[i].GetPosition(), toPoint);
if (distance < closest)
{
closest = distance;
}
}

return closest;
}

protected bool IsTargetAlive()
{
EntityAI entity;
if (Class.CastTo(entity, m_EscortTarget))
{
return entity.IsAlive();
}

return false;
}

protected void SpawnPresetAI(vector position)
{
Print(string.Format("[DAM] Spawning escort ambush AI at %1", position.ToString()));
SpawnAIAt(position, new eAIFactionLunatics());
}
}

class DAMVehicleDefenseMission : DAMMissionBase
{
protected Object m_TargetVehicle;
protected float m_AttackRadius = 1000.0;
protected ref array<Object> m_ActiveAttackers = new array<Object>();
protected ref DAMVehicleRewardSettings m_VehicleSettings;
protected float m_ReinforcementInterval = 45.0;
protected float m_ReinforcementTimer;

void DAMVehicleDefenseMission(string missionId, DAMTierSettings tier, vector origin, DAMMissionMessages messages, DAMVehicleRewardSettings vehicleSettings, float timeLimit, DAMOverhaulConfig config = null)
{
super.DAMMissionBase(missionId, tier, origin, messages, timeLimit, config);
m_VehicleSettings = vehicleSettings;
}

override protected void OnStart()
{
SpawnVehicle();
SpawnInitialWave();
m_ReinforcementTimer = m_ReinforcementInterval;
}

override protected void OnTick(float deltaTime)
{
if (!m_TargetVehicle)
{
FailMission("Vehicle destroyed");
return;
}

if (IsVehicleDestroyed())
{
FailMission("Vehicle destroyed");
return;
}

CleanupDeadAttackers();
m_ReinforcementTimer -= deltaTime;
if (m_ReinforcementTimer <= 0)
{
SpawnReinforcements();
m_ReinforcementTimer = m_ReinforcementInterval;
}

if (m_ActiveAttackers.Count() == 0)
{
CompleteMission();
}
}

override protected void OnComplete()
{
if (!m_VehicleSettings.IncludeVehicleAsReward && m_TargetVehicle)
{
GetGame().ObjectDelete(m_TargetVehicle);
}
}

override protected void OnFail(string reason)
{
if (m_TargetVehicle && !m_VehicleSettings.IncludeVehicleAsReward)
{
GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DeleteVehicle, (int)(m_VehicleSettings.DespawnDelayOnFailure * 1000), false);
}
}

protected void SpawnVehicle()
{
m_TargetVehicle = GetGame().CreateObject("OffroadHatchback", m_Location, true, false, true);
if (m_TargetVehicle)
{
Print(string.Format("[DAM] Spawned defense vehicle for %1 at %2", m_MissionId, m_Location.ToString()));
}
}

protected void SpawnInitialWave()
{
for (int i = 0; i < m_Tier.EnemyCount; i++)
{
vector spawnPos = FindRandomizedPosition(m_Location, m_AttackRadius);
SpawnAttacker(spawnPos);
}
}

protected void SpawnAttacker(vector position)
{
Print(string.Format("[DAM] Spawning attacker for vehicle mission at %1", position.ToString()));
Object attacker = SpawnAIAt(position, new eAIFactionLunatics());
if (attacker)
{
m_ActiveAttackers.Insert(attacker);
}
}

protected bool IsVehicleDestroyed()
{
Transport transport;
if (Class.CastTo(transport, m_TargetVehicle))
{
return transport.IsDestroyed();
}

return true;
}

protected void CleanupDeadAttackers()
{
for (int i = m_ActiveAttackers.Count() - 1; i >= 0; i--)
{
EntityAI attacker;
if (!Class.CastTo(attacker, m_ActiveAttackers[i]) || !attacker.IsAlive())
{
m_ActiveAttackers.Remove(i);
}
}
}

protected void SpawnReinforcements()
{
int waveSize = Math.Max(1, m_Tier.EnemyCount / 3);
for (int i = 0; i < waveSize; i++)
{
vector spawnPos = FindRandomizedPosition(m_Location, m_AttackRadius);
SpawnAttacker(spawnPos);
}
}

protected void DeleteVehicle()
{
GetGame().ObjectDelete(m_TargetVehicle);
}
}

class DAMTerritoryWarMission : DAMMissionBase
{
protected ref array<Object> m_FactionA = new array<Object>();
protected ref array<Object> m_FactionB = new array<Object>();
protected float m_ReinforcementDelay = 120.0;
protected float m_ReinforcementTimer;
protected string m_FactionAClassName;
protected string m_FactionBClassName;

void DAMTerritoryWarMission(string missionId, DAMTierSettings tier, vector origin, DAMMissionMessages messages, DAMOverhaulConfig config = null, string factionAClass = "", string factionBClass = "")
{
super.DAMMissionBase(missionId, tier, origin, messages, 0, config);
m_FactionAClassName = factionAClass;
m_FactionBClassName = factionBClass;
}

override protected void OnStart()
{
SpawnFaction(m_FactionA, "FactionA");
SpawnFaction(m_FactionB, "FactionB");
m_ReinforcementTimer = m_ReinforcementDelay;
}

override protected void OnTick(float deltaTime)
{
if (IsFactionDefeated(m_FactionA) && IsFactionDefeated(m_FactionB))
{
CompleteMission();
return;
}

m_ReinforcementTimer -= deltaTime;
if (m_ReinforcementTimer <= 0)
{
SpawnFaction(m_FactionA, "FactionA");
SpawnFaction(m_FactionB, "FactionB");
m_ReinforcementTimer = m_ReinforcementDelay;
}
}

override protected void OnComplete()
{
Print(string.Format("[DAM] Territory War mission %1 finished after all AI were eliminated", m_MissionId));
}

protected void SpawnFaction(ref array<Object> factionArray, string factionName)
{
for (int i = 0; i < m_Tier.EnemyCount; i++)
{
vector position = FindRandomizedPosition(m_Location, m_Messages.MarkerRadius);
Object ai = SpawnFactionAI(position, factionName);
if (ai)
{
factionArray.Insert(ai);
}
}
}

protected Object SpawnFactionAI(vector position, string factionName)
{
Print(string.Format("[DAM] Spawning %1 soldier at %2", factionName, position.ToString()));
eAIFaction faction = CreateFaction(factionName);
return SpawnAIAt(position, faction);
}

protected eAIFaction CreateFaction(string factionIdentifier)
{
if (factionIdentifier == "FactionA" && m_FactionAClassName != "")
{
return InstantiateFactionByName(m_FactionAClassName);
}

if (factionIdentifier == "FactionB" && m_FactionBClassName != "")
{
return InstantiateFactionByName(m_FactionBClassName);
}

if (factionIdentifier == "FactionA")
{
return new eAIFactionLunatics();
}

return new eAIFactionInfected();
}

protected eAIFaction InstantiateFactionByName(string className)
{
switch (className)
{
case "eAIFactionLunatics":
return new eAIFactionLunatics();
case "eAIFactionInfected":
return new eAIFactionInfected();
case "eAIFactionBandit":
return new eAIFactionBandit();
default:
Print(string.Format("[DAM] Unknown faction class %1, defaulting to Lunatics", className));
return new eAIFactionLunatics();
}
}

protected bool IsFactionDefeated(ref array<Object> factionArray)
{
for (int i = factionArray.Count() - 1; i >= 0; i--)
{
Object ai = factionArray[i];
EntityAI entity;
if (Class.CastTo(entity, ai) && entity.IsAlive())
{
return false;
}

factionArray.Remove(i);
}

return true;
}
}

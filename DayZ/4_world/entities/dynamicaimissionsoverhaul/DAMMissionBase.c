class DAMMissionBase
{
protected string m_MissionId;
protected ref DAMMissionMessages m_Messages;
protected ref DAMTierSettings m_Tier;
protected vector m_Location;
protected bool m_IsActive;
protected float m_TimeLimit;
protected float m_Elapsed;

void DAMMissionBase(string missionId, DAMTierSettings tier, vector origin, DAMMissionMessages messages, float timeLimit = 1800)
{
m_MissionId = missionId;
m_Tier = tier;
m_Location = origin;
m_Messages = messages;
m_TimeLimit = timeLimit;
m_Elapsed = 0;
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
player.MessageStatus(message);
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

protected void OnComplete()
{
}

protected void OnFail(string reason)
{
}
}

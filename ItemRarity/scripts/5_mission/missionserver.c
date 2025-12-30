modded class MissionServer
{
    override void OnClientPrepareEvent(PlayerIdentity identity, out bool useDB, out vector pos, out float yaw, out int preloadTimeout)
	{
        super.OnClientPrepareEvent(identity, useDB, pos, yaw, preloadTimeout);

        GetRPCManager().SendRPC("ItemRarity", "OnItemRarityConfigReceived", new Param1<ItemRarityConfig>(GetRarityConfig()), true, identity);
    }
}
modded class MissionServer
{
    override void OnClientPrepareEvent(PlayerIdentity identity, out bool useDB, out vector pos, out float yaw, out int preloadTimeout)
    {
        super.OnClientPrepareEvent(identity, useDB, pos, yaw, preloadTimeout);

        GetRPCManager().SendRPC("ItemTags", "OnItemTagsConfigReceived", new Param1<ItemTagsConfig>(GetItemTagsConfig()), true, identity);
    }
}

modded class ModItemRegisterCallbacks
{
        override void RegisterOneHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
        {
                super.RegisterOneHanded(pType, pBehavior);
                pType.AddItemInHandsProfileIK("ColdPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/Heatpack.anm");
        }
}

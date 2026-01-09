modded class ModItemRegisterCallbacks
{
    override void RegisterTwoHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
    { 
  
	super.RegisterTwoHanded(pType, pBehavior);
//	pType.AddItemInHandsProfileIK("TP_Tablet", "dz/anims/workspaces/player/player_main/player_main_2h.asi", 			pBehavior , 	"drone/anm/tablet_IK.anm");


	pType.AddItemInHandsProfileIK("TP_Tablet", "drone/anm/tablet.asi", 			pBehavior , 	"drone/anm/tablet_Click.anm");


	//pType.AddItemInHandsProfileIK("TP_Tablet", "dz/anims/workspaces/player/player_main/props/player_main_2h_cauldron.asi", 			pBehavior , 	"Tablet/Shovel_IK.anm");


    }	

};

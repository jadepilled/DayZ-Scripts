class DeliveryCrate	: WoodenCrate
{
	
	
};


class DeliveryCrate_Yellow	: Container_Base
{
    override bool CanPutInCargo(EntityAI parent) {
        return false;
    }

    override bool CanPutIntoHands(EntityAI parent) {
        return false;
    }	
	
	override void SetActions()
	{
	}	
};



class TP_Tablet : GPSReceiver
{
	
	override void OnWorkStart(){}	
	override void OnWorkStop(){}	
	override void OnWork(float consumed_energy){}	
	
	override void SetActions()
	{
		AddAction(ActionTakeItem);
		AddAction(ActionTakeItemToHands);
		AddAction(ActionWorldCraft);
		AddAction(ActionDropItem);
		AddAction(ActionAttachWithSwitch);
//  	  AddAction(ActionCallDroneTest);
		AddAction(ActionTurnOnTablet);
		AddAction(ActionTurnOffTablet);	 	  
		AddAction(ActionOpenDroneShop);		
	}		
};


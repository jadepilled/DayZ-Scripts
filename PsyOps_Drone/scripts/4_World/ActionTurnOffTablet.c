class ActionTurnOffTablet : ActionSingleUseBase
{
	void ActionTurnOffTablet()
	{
		m_Text = "#switch_off";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem 	= new CCINonRuined();
		m_ConditionTarget 	= new CCTNone();
	}

	override bool HasTarget()
	{
		return false;
	}
	
	override bool HasProneException()
	{
		return true;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (item.IsInherited(GPSReceiver))
		{
			m_CommandUID 		= DayZPlayerConstants.CMD_ACTIONMOD_PRESS_TRIGGER;
			m_CommandUIDProne 	= DayZPlayerConstants.CMD_ACTIONFB_PRESS_TRIGGER;
		}

		
		return item.HasEnergyManager() && item.GetCompEM().CanSwitchOff();
	}



	override void OnExecuteServer(ActionData action_data)
	{
		action_data.m_MainItem.GetCompEM().SwitchOff();

		action_data.m_MainItem.SetObjectTexture(0, "PsyOps_Drone\\data\\tablet_off_co.paa");
	}

}

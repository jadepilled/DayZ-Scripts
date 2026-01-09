class ActionOpenDroneShop : ActionSingleUseBase
{
    void ActionOpenDroneShop()
    {
        m_Text = "Open Drone Shop";
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PRESS_TRIGGER;
        m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_PRESS_TRIGGER;
    }

    override void CreateConditionComponents()  
    {   
        m_ConditionItem  = new CCINonRuined();
        m_ConditionTarget  = new CCTNone();
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
		//return item.IsTurnedOn();
		return item.GetCompEM() && item.GetCompEM().IsWorking()&& item.IsInherited(TP_Tablet);
       // return item && item.IsInherited(TP_Tablet);  
    }

	override void OnExecuteServer(ActionData action_data)
	{
		super.OnExecuteServer(action_data);
			Param1<int> param = new Param1<int>(1);
			GetRPCManager().SendRPC("drone", "OpenDroneBuyMenuUI", param, true, action_data.m_Player.GetIdentity());
	}

    override void OnExecuteClient(ActionData action_data)
    {
        super.OnExecuteClient(action_data);

    }
}

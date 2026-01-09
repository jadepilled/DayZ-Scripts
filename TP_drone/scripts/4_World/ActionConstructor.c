modded class ActionConstructor 
{

    override void RegisterActions(TTypenameArray actions)
    {
		super.RegisterActions(actions);
        //actions.Insert(ActionCallDroneTest);
        actions.Insert(ActionOpenDroneShop);
		actions.Insert(ActionTurnOnTablet);
		actions.Insert(ActionTurnOffTablet); 
    }
}
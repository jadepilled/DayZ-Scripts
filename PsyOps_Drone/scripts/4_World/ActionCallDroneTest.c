class ActionCallDroneTest : ActionSingleUseBase
{
    void ActionCallDroneTest()
    {
        m_Text = "Call Drone";
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
        return false;  // This action doesn't need a specific target
    }

    override bool HasProneException()
    {
        return true;  // Allow action in prone position
    }

    // Condition to check if the player is holding the correct item (in this case, Paper)
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        return item && item.IsInherited(GPSReceiver);  // You can change "Paper" to any other item you want to trigger the action with
    }

override void OnExecuteServer(ActionData action_data)
{
    super.OnExecuteServer(action_data);

    Print("ActionCallDroneTest: OnExecuteServer called.");  // Debug print to check if the server-side code is running

    PlayerBase targetPlayer = PlayerBase.Cast(action_data.m_Player);

    if (targetPlayer)
    {
        Print("ActionCallDroneTest: Target player found.");  // Debug: Check if player is valid

        // Get the player's position
        vector playerPos = targetPlayer.GetPosition();

        // Generate a random angle in radians
        float randomAngle = Math.RandomFloatInclusive(0, 360) * Math.DEG2RAD;

        // Generate a random distance between 0 and 50 meters
        float randomDistance = Math.RandomFloatInclusive(0, 50);

        // Calculate the offset from the player's position
        float offsetX = randomDistance * Math.Cos(randomAngle);
        float offsetZ = randomDistance * Math.Sin(randomAngle);

        // Calculate the spawn position with the offset
        vector spawnPosition = Vector(playerPos[0] + offsetX, 0, playerPos[2] + offsetZ);

        // Get the ground height at the spawn position
        float groundY = GetGame().SurfaceY(spawnPosition[0], spawnPosition[2]);

        // Set the Y coordinate to 99 meters above the ground
        spawnPosition[1] = groundY + 99;

        // Define the loot array that will be placed inside the drone's crate
        array<ref DeliveryKitItem> lootArray = { new DeliveryKitItem("AKM", 1), new DeliveryKitItem("AmmoBox", 1), new DeliveryKitItem("CanOpener", 1), new DeliveryKitItem("Canteen", 1) };

        // Create a new DroneSpawner instance
        DroneSpawner spawner = new DroneSpawner();

        // Spawn the drone, assign the target player, and add loot to the attached crate
        spawner.SpawnDrone(targetPlayer, spawnPosition, lootArray);

        Print("ActionCallDroneTest: Drone should be spawned.");  // Debug: Check if the spawn function was called
    }
    else
    {
        Print("ActionCallDroneTest: Target player not found.");  // Debug: If no player is found
    }
}



    // Client-side logic: Generally for effects or UI feedback, which you might want to add later
    override void OnExecuteClient(ActionData action_data)
    {
        super.OnExecuteClient(action_data);
        // For now, we leave this empty since most of the action is server-side
    }
}

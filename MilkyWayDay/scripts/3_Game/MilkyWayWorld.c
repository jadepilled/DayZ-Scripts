/**
 * Helper used by the mission layer to repeatedly assert the desired sky settings.
 */
class MilkyWayDayController
{
    static void ForceSpaceVisibility()
    {
        World world = g_Game.GetWorld();
        if (!world)
            return;

        Param1<bool> visible = new Param1<bool>(true);

        // Use runtime reflection instead of modding the World engine class.
        GetGame().GameScript.CallFunctionParams(world, "SetMilkyWayVisibility", null, visible);
        GetGame().GameScript.CallFunctionParams(world, "SetSpaceObjectVisibility", null, visible);
    }
}

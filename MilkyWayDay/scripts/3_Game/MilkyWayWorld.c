/**
 * Helper used by the mission layer to repeatedly assert the desired sky settings.
 */
class MilkyWayDayController
{
    protected static void CallWorldFunction(string func, Param params)
    {
        World world = g_Game.GetWorld();
        if (!world)
            return;

        GetGame().GameScript.CallFunctionParams(world, func, null, params);
    }

    static void ForceSpaceVisibility()
    {
        // Keep the sky objects fully enabled and bright enough to overcome the daytime fade-out.
        CallWorldFunction("SetMilkyWayVisibility", new Param1<bool>(true));
        CallWorldFunction("SetSpaceObjectVisibility", new Param1<bool>(true));

        CallWorldFunction("SetMilkyWayIntensity", new Param1<float>(1.0));
        CallWorldFunction("SetSpaceObjectIntensity", new Param1<float>(1.0));
        CallWorldFunction("SetSpaceObjectGlow", new Param1<float>(1.0));
        CallWorldFunction("SetStarsIntensity", new Param1<float>(1.0));
    }
}

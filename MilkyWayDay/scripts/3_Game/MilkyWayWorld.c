modded class World
{
    // Native hooks that control whether the sky dome renders the milky way band.
    proto native void SetMilkyWayVisibility(bool visible);

    // Native hook that toggles the generic space object/skybox visibility.
    proto native void SetSpaceObjectVisibility(bool visible);
}

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

        world.SetMilkyWayVisibility(true);
        world.SetSpaceObjectVisibility(true);
    }
}

modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();
        MilkyWayDayController.ForceSpaceVisibility();
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        MilkyWayDayController.ForceSpaceVisibility();
    }
}

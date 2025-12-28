modded class MissionServer
{
    protected ref DAMEventDirector m_DAMDirector;

    override void OnInit()
    {
        super.OnInit();
        m_DAMDirector = new DAMEventDirector();
        Print("[DAM] Dynamic AI Missions overhaul director initialized (AI_Missions addon)");
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        if (m_DAMDirector)
        {
            m_DAMDirector.Tick(timeslice);
        }
    }

    override void OnMissionFinish()
    {
        super.OnMissionFinish();
        m_DAMDirector = null;
    }
}

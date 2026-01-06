modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();
        SkyRemasterLighting.Apply();
        SkyRemasterSky.Apply();
    }
}

modded class MissionGameplay
{
    override void OnInit()
    {
        super.OnInit();
        SkyRemasterLighting.Apply();
        SkyRemasterSky.Apply();
    }
}

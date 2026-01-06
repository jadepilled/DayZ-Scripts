class SkyRemasterSky
{
    protected static bool m_Applied;
    protected static const string SKY_PRESET_FILE = "SkyRemaster/data/sky/sky_presets.xml";
    protected static const string SKY_PRESET_NAME = "SkyRemaster";

    static void Apply()
    {
        if (m_Applied)
            return;

        m_Applied = true;

        if (!FileExist(SKY_PRESET_FILE))
        {
            Print("[SkyRemaster] Missing sky preset file: " + SKY_PRESET_FILE);
            return;
        }

        int presetCount = LoadSkyPresets(SKY_PRESET_FILE);
        if (presetCount <= 0)
        {
            Print("[SkyRemaster] Failed to load presets from: " + SKY_PRESET_FILE);
            return;
        }

        float dayTime = g_Game.GetDayTime() / 24.0;
        dayTime = Math.Clamp(dayTime, 0.0, 1.0);

        InitSky(SKY_PRESET_NAME);
        SetSkyPreset(SKY_PRESET_NAME, 0.0, dayTime);
    }
}

modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();
        SkyRemasterSky.Apply();
    }
}

modded class MissionGameplay
{
    override void OnInit()
    {
        super.OnInit();
        SkyRemasterSky.Apply();
    }
}

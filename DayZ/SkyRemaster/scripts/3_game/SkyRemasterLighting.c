class SkyRemasterLighting
{
    protected static bool m_Applied;
    protected static const string LIGHTING_PROFILE_PATH = "SkyRemaster/data/lighting/skyremaster_lighting.json";
    protected static const string LIGHTING_PROFILE_NAME = "SkyRemaster";

    static void Apply()
    {
        if (m_Applied)
            return;

        m_Applied = true;

        // User lighting configs are evaluated client-side and control the final skybox shading.
        if (!FileExist(LIGHTING_PROFILE_PATH))
        {
            Print("[SkyRemaster] Missing lighting profile: " + LIGHTING_PROFILE_PATH);
            return;
        }

        GetGame().GetWorld().LoadUserLightingCfg(LIGHTING_PROFILE_PATH, LIGHTING_PROFILE_NAME);
        GetGame().GetWorld().SetUserLightingLerp(1.0);
    }
}

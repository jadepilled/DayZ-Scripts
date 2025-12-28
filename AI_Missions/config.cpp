class CfgPatches
{
    class AI_Missions
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data", "DZ_Scripts"};
    };
};

class CfgMods
{
    class AI_Missions
    {
        dir = "AI_Missions";
        name = "Dynamic AI Missions Overhaul";
        author = "psyopgirl";
        version = "1.0";
        type = "mod";
        dependencies[] = {"Game", "World", "Mission"};

        class defs
        {
            class gameScriptModule
            {
                files[] = {"AI_Missions/scripts/3_Game"};
            };

            class worldScriptModule
            {
                files[] = {"AI_Missions/scripts/4_World"};
            };

            class missionScriptModule
            {
                files[] = {"AI_Missions/scripts/5_Mission"};
            };
        };
    };
};

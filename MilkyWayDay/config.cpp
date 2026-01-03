class CfgPatches
{
    class MilkyWayDay
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data"};
    };
};

class CfgMods
{
    class MilkyWayDay
    {
        dir = "MilkyWayDay";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "MilkyWayDay";
        credits = "";
        author = "";
        authorID = "";
        version = "1.0";
        extra = 0;
        type = "mod";

        dependencies[] = {"Game", "Mission"};

        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = {"MilkyWayDay/scripts/3_Game"};
            };
            class missionScriptModule
            {
                value = "";
                files[] = {"MilkyWayDay/scripts/5_Mission"};
            };
        };
    };
};

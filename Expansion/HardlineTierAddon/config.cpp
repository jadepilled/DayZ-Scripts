class CfgPatches
{
    class DZ_Expansion_Hardline_TierAddon
    {
        requiredAddons[] = {"DZ_Expansion_Hardline"};
        units[] = {};
        weapons[] = {};
    };
};

class CfgMods
{
    class Expansion_Hardline_TierAddon
    {
        dir = "Expansion_Hardline_TierAddon";
        name = "Expansion Hardline Tier Addon";
        author = "ServerAdmin";
        dependencies[] = {"Game", "World", "Mission"};

        class defs
        {
            class gameScriptModule
            {
                files[] = {"Expansion/HardlineTierAddon/scripts/3_Game"};
            };

            class worldScriptModule
            {
                files[] = {"Expansion/HardlineTierAddon/scripts/4_World"};
            };

            class missionScriptModule
            {
                files[] = {"Expansion/HardlineTierAddon/scripts/5_Mission"};
            };
        };
    };
};

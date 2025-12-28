class CfgPatches
{
    class ExpansionHardlineTierAddon
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DayZExpansion_Hardline_Scripts"};
    };
};

class CfgMods
{
    class ExpansionHardlineTierAddon
    {
        dir = "HardlineTierAddon";
        picture = "";
        hideName = 1;
        hidePicture = 1;
        name = "DayZ Expansion Hardline Tier Addon";
        credits = "";
        author = "";
        version = "1.0";
        type = "mod";

        dependencies[] = {"Game"};

        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = {"HardlineTierAddon/hardline_tiers_scripts/3_Game"};
            };
        };
    };
};

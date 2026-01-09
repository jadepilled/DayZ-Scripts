#define _ARMA_

class CfgPatches
{
    class Cyberware_Items_Nervous_System_Kerenzikov
    {
        units[] = {"Kerenzikov"};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data","DZ_Scripts","Cyberware_Slots"};
    };
};

class CfgMods
{
    class Cyberware_Items_Nervous_System_Kerenzikov
    {
        dir = "Cyberware_Items_Nervous_System/Kerenzikov";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "Cyberware_Items_Nervous_System_Kerenzikov";
        credits = "";
        author = "psyopgirl";
        authorID = "";
        version = 1;
        extra = 0;
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                files[] = {"Cyberware_Items_Nervous_System/Kerenzikov/scripts/4_World"};
            };
        };
    };
};

class CfgVehicles
{
    class Clothing;

    class CyberwareNervousBase: Clothing
    {
        scope = 0;
        weight = 200;
        itemSize[] = {1,2};
        inventorySlot[] = {};
        simulation = "clothing";
        model = "dz\\gear\\medical\\injector.p3d";
        debug_ItemCategory = 1;
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints = 100;
                    healthLevels[] =
                    {
                        {1.0,{"DZ\\gear\\consumables\\data\\bandana_rag.rvmat"}},
                        {0.7,{"DZ\\gear\\consumables\\data\\bandana_rag.rvmat"}},
                        {0.5,{"DZ\\gear\\consumables\\data\\bandana_rag.rvmat"}},
                        {0.3,{"DZ\\gear\\consumables\\data\\bandana_rag.rvmat"}},
                        {0.0,{"DZ\\gear\\consumables\\data\\bandana_rag.rvmat"}}
                    };
                };
            };
        };
    };

    class Kerenzikov: CyberwareNervousBase
    {
        scope = 2;
        displayName = "Kerenzikov";
        descriptionShort = "Cyberware that reduces stamina depletion while attached.";
        inventorySlot[] = {"Cyberware_Nervous_System"};
    };
};

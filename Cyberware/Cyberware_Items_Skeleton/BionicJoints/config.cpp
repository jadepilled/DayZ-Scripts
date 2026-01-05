#define _ARMA_

class CfgPatches
{
    class Cyberware_Items_Skeleton_BionicJoints
    {
        units[] = {"BionicJoints"};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data","DZ_Scripts","Cyberware_Slots"};
    };
};

class CfgMods
{
    class Cyberware_Items_Skeleton_BionicJoints
    {
        dir = "Cyberware_Items_Skeleton/BionicJoints";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "Cyberware_Items_Skeleton_BionicJoints";
        credits = "";
        author = "psyopgirl";
        authorID = "";
        version = 1;
        extra = 0;
        type = "mod";
    };
};

class CfgVehicles
{
    class Clothing;

    class CyberwareSkeletonBase: Clothing
    {
        scope = 0;
        weight = 250;
        itemSize[] = {1,2};
        inventorySlot[] = {};
        simulation = "clothing";
        model = "dz\\gear\\tools\\pliers.p3d";
        debug_ItemCategory = 1;
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints = 120;
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
            class GlobalArmor
            {
                class Melee
                {
                    class Health{damage = 0.85;};
                    class Blood{damage = 0.85;};
                    class Shock{damage = 0.85;};
                };
                class Projectile
                {
                    class Health{damage = 0.85;};
                    class Blood{damage = 0.85;};
                    class Shock{damage = 0.85;};
                };
                class FragGrenade
                {
                    class Health{damage = 0.85;};
                    class Blood{damage = 0.85;};
                    class Shock{damage = 0.85;};
                };
            };
        };
    };

    class BionicJoints: CyberwareSkeletonBase
    {
        scope = 2;
        displayName = "Bionic Joints";
        descriptionShort = "Reinforced joints that provide modest armour bonuses.";
        inventorySlot[] = {"Cyberware_Skeleton"};
    };
};

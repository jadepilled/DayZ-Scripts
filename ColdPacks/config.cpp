class CfgPatches
{
        class ColdPacks
        {
                units[]={"ColdPack"};
                weapons[] = {};
                requiredVersion = 0.1;
                requiredAddons[] = {"DZ_Data","DZ_Gear_Consumables"};
        };
};

class CfgMods
{
        class ColdPacks
        {
                dir = "ColdPacks";
                picture = "";
                action = "";
                hideName = 1;
                hidePicture = 1;
                name = "Cold Packs";
                credits = "";
                author = "";
                authorID = "";
                version = "1.0";
                type = "mod";
                dependencies[] = {"World"};
                class defs
                {
                        class worldScriptModule
                        {
                                value = "";
                                files[] = {"ColdPacks/scripts/4_world"};
                        };
                };
        };
};

class CfgVehicles
{
        class Heatpack;
        class ColdPack: Heatpack
        {
                scope = 2;
                displayName = "Cold Pack";
                descriptionShort = "A cold pack that cools nearby objects and players.";
                model = "\dz\gear\consumables\Heatpack.p3d";
                hiddenSelectionsTextures[] = {"ColdPacks\data\tex\PsyOps_ColdPack_co.paa"};
        };
};

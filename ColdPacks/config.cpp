class CfgPatches
{
        class ColdPacks
        {
                author = "psyopgirl";
		name = "Cold Pack";
		url = "https://psyops.gg";
		units[]={"ColdPack"};
		weapons[] = {};
		requiredVersion = 0.1;
                requiredAddons[] = {"DZ_Data","DZ_Characters_Backpacks","DZ_Characters_Gloves","DZ_Characters_Headgear","DZ_Characters_Masks","DZ_Characters_Pants","DZ_Characters_Shoes","DZ_Characters_Tops","DZ_Characters_Vests","DZ_Characters_Zombies","DZ_Gear_Books","DZ_Gear_Camping","DZ_Gear_Consumables","DZ_Gear_Containers","DZ_Gear_Cooking","DZ_Gear_Crafting","DZ_Gear_Drinks","DZ_Gear_Food","DZ_Gear_Medical","DZ_Gear_Navigation","DZ_Gear_Tools","DZ_Radio","DZ_Scripts","DZ_Vehicles_Parts","DZ_Weapons_Ammunition"};
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
        class Inventory_Base;

        class Coldpack: Inventory_Base
	{
		scope=2;
		displayName = "Cold Pack";
                descriptionShort = "A single-use cold pack that can be activated by squeezing the container. Can be used to alleviate pain from swelling or used to cool down.";
		model="\DZ\gear\tools\Heatpack.p3d";
                hiddenSelections[] = {"zbytek"};
			hiddenSelectionsTextures[] = {"ColdPacks\data\tex\PsyOps_Cold_Pack_co.paa",
			"ColdPacks\data\tex\PsyOps_Cold_Pack_co.paa",
			"ColdPacks\data\tex\PsyOps_Cold_Pack_co.paa"
			};
		animClass="Knife";
		rotationFlags=17;
		itemSize[]={1,2};
		varEnergyInit=3000;
		varEnergyMin=0;
		varEnergyMax=3000;
		weightPerQuantityUnit=0;
		temperaturePerQuantityWeight=4;
		varTemperatureMax=20;
		varTemperatureMin=-20;
		weight=150;
		fragility=0.0099999998;
		soundImpactType="plastic";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=30;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\gear\tools\data\heat_pack.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\gear\tools\data\heat_pack.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\gear\tools\data\heat_pack_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\gear\tools\data\heat_pack_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\gear\tools\data\heat_pack_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class EnergyManager
		{
			energyAtSpawn=3600;
			energyUsagePerSecond=1;
			updateInterval=40;
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet="pickUpBloodBag_SoundSet";
					id=797;
				};
				class drop
				{
					soundset="bloodbag_drop_SoundSet";
					id=898;
				};
			};
		};
	};

};

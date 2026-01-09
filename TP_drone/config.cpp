#define _ARMA_

class CfgPatches
{
	class TP_drone
	{
		units[] = {"TP_Drone","mb_Shield","DeliveryCrate","DeliveryCrate_Yellow","TP_Tablet"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Scripts","DZ_Sounds_Effects","DZ_Gear_Navigation","DZ_Gear_Camping","DZ_Gear_Containers","DZ_Gear_Tools"};
	};
};
class CfgMods
{
	class TP_drone
	{
		dir = "drone";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "TP_drone";
		credits = "";
		author = "psyopgirl";
		authorID = "";
		version = 1;
		extra = 0;
		type = "mod";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = {"drone/scripts/4_World"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"drone/scripts/3_Game"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"drone/scripts/5_Mission"};
			};
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class Paper;
	class GPSReceiver;
	class CarBattery;
	class WoodenCrate;
	class Container_Base;
	class TP_Drone: Inventory_Base
	{
		scope = 2;
		displayName = "TP Drone";
		descriptionShort = "";
		model = "drone\data\drone_shield.p3d";
		itemSize[] = {4,4};
		weight = 100;
		hiddenSelections[] = {"shield"};
		hiddenSelectionsTextures[] = {"drone\data\hex_full.paa"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1000;
					healthLevels[] = {{1.0,{"DZ\characters\masks\Data\Vintagehockeym.rvmat"}},{0.7,{"DZ\characters\masks\Data\Vintagehockeym.rvmat"}},{0.5,{"DZ\characters\masks\Data\Vintagehockeym_damage.rvmat"}},{0.3,{"DZ\characters\masks\Data\Vintagehockeym_damage.rvmat"}},{0.0,{"DZ\characters\masks\Data\Vintagehockeym_destruct.rvmat"}}};
				};
			};
		};
		class AnimationSources
		{
			class propeller_1
			{
				source = "time";
				initPhase = 0;
				animPeriod = 0.5;
			};
		};
	};
	class mb_Shield: Inventory_Base
	{
		scope = 2;
		displayName = "mb_Shield";
		descriptionShort = "";
		model = "drone\shield.p3d";
		hiddenSelections[] = {"shield"};
		hiddenSelectionsTextures[] = {"drone\data\hex_full.paa"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 9000;
					healthLevels[] = {{1.0,{"DZ\characters\masks\Data\Vintagehockeym.rvmat"}},{0.7,{"DZ\characters\masks\Data\Vintagehockeym.rvmat"}},{0.5,{"DZ\characters\masks\Data\Vintagehockeym_damage.rvmat"}},{0.3,{"DZ\characters\masks\Data\Vintagehockeym_damage.rvmat"}},{0.0,{"DZ\characters\masks\Data\Vintagehockeym_destruct.rvmat"}}};
				};
			};
		};
	};
	class DeliveryCrate: WoodenCrate
	{
		scope = 2;
		displayName = "DeliveryCrate";
		model = "drone\data\drone_crate.p3d";
		descriptionShort = "";
		class Cargo
		{
			itemsCargoSize[] = {10,100};
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
	};
	class DeliveryCrate_Yellow: Container_Base
	{
		scope = 2;
		displayName = "PsyOps Delivery Crate";
		model = "drone\data\drone_crate.p3d";
		descriptionShort = "PsyOps Corporation Standard Delivery Crate.";
		class Cargo
		{
			itemsCargoSize[] = {10,100};
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
	};
	class TP_Tablet: GPSReceiver
	{
		scope = 2;
		model = "drone\data\tablet.p3d";
		displayName = "PsyOps Drone Tablet";
		descriptionShort = "A tablet with the PsyOps Corp logo on the back. Can be used to call drone deliveries.";
		itemSize[] = {2,1};
		weight = 0.5;
		rotationFlags = 63;
		hiddenSelections[] = {"screen"};
		hiddenSelectionsTextures[] = {"drone\data\tablet_off_co.paa"};
		inventorySlot[] = {""};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
		};
	};
};
class CfgSoundShaders
{
	class drone_soundshader
	{
		samples[] = {{"drone\sounds\drone.ogg",1}};
		range = 150;
		volume = 2;
	};
};
class CfgSoundSets
{
	class drone_soundset
	{
		soundShaders[] = {"drone_soundshader"};
	};
};

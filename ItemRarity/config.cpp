class CfgMods
{
	class ItemRarity
	{
		dir="ItemRarity";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="Item Rarity by BP";
		credits="BP";
		author="Maroonchy";
		authorID="";
		version="Version 1.0";
		extra=0;
		type="mod";
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"ItemRarity/scripts/3_game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"ItemRarity/scripts/4_world"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"ItemRarity/scripts/5_mission"
				};
			};
		};
	};
};
class CfgPatches
{
	class ItemRarity
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};

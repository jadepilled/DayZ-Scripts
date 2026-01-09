class CfgMods
{
	class BetterBoltAction
	{
		dir="BetterBoltAction";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="Better Bolt Action";
		author="OpenAI";
		authorID="0";
		version="1.0";
		extra=0;
		type="mod";
		dependencies[]=
		{
			"World"
		};
		class defs
		{
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"BetterBoltAction/scripts/4_world"
				};
			};
		};
	};
};
class CfgPatches
{
	class BetterBoltAction
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

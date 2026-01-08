class CfgMods
{
	class ADSReload
	{
		dir="ADSReload";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="ADS Reload";
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
					"ADSReload/scripts/4_world"
				};
			};
		};
	};
};
class CfgPatches
{
	class ADSReload
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

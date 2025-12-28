class CfgPatches
{
	class AI_Missions
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DayZExpansion_Core_Scripts",
			"DayZExpansion_AI_Scripts"
		};
	};
};
class CfgMods
{
	class AI_Missions
	{
		dir="AI_Missions";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="AI_Missions";
		logoSmall="";
		logo="";
		logoOver="";
		tooltip="";
		overview="Mission was developed by Touchofdeath, adaptation for Expansion markers and server-side addon by Sid Debian, modified by psyopgirl";
		credits="";
		author="Touchofdeath & Sid Debian [76561197991259240]";
		authorID="0";
		version="2.0";
		extra=1;
		type="mod";
		dependencies[]=
		{
			"Game",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"AI_Missions/Scripts/3_Game"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"AI_Missions/Scripts/5_Mission"
				};
			};
		};
	};
};

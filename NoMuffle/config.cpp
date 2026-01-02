class CfgPatches
{
	class NoMuffle
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Characters",
			"DZ_Characters_Masks",
			"DZ_Characters_Headgear"
		};
	};
};
class CfgMods
{
	class NoMuffle
	{
		dir="NoMuffle";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="NoMuffle Module";
		author="Zedmag";
		authorID="0";
		version="3.0";
		extra=0;
		type="mod";
		dependencies[]=
		{
			"Game",
			"world"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"NoMuffle/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"NoMuffle/scripts/4_world"
				};
			};
		};
	};
};
class CfgVehicles
{
	class clothing;
	class GasMask: clothing
	{
		soundVoiceType="none";
		soundVoicePriority=3;
	};
	class GP5GasMask: clothing
	{
		soundVoiceType="none";
		soundVoicePriority=3;
	};
	class DarkMotoHelmet_ColorBase: clothing
	{
		soundVoiceType="none";
		soundVoicePriority=3;
	};
	class MotoHelmet_ColorBase: clothing
	{
		soundVoiceType="none";
		soundVoicePriority=3;
	};
};
class CfgNonAIVehicles
{
	class StaticObject;
};

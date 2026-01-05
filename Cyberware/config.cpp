#define _ARMA_

class CfgPatches
{
	class Cyberware
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Scripts","DZ_Characters","DZ_Characters_Headgear","DZ_Characters_Backpacks"};
	};
};
class CfgMods
{
	class Cyberware
	{
		dir = "Cyberware";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "Cyberware";
		credits = "";
		author = "psyopgirl";
		authorID = "";
		version = 1;
		extra = 0;
		type = "mod";
	};
};
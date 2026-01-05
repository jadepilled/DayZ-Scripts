#define _ARMA_

class CfgPatches
{
	class Cyberware_Slots
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Scripts","DZ_Characters","DZ_Characters_Headgear","DZ_Characters_Backpacks"};
	};
};
class CfgMods
{
	class Cyberware_Slots
	{
		dir = "Cyberware_Slots";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "Cyberware_Slots";
		credits = "";
		author = "psyopgirl";
		authorID = "";
		version = 1;
		extra = 0;
		type = "mod";
		class defs
		{
			class imageSets
			{
				files[] = {"Cyberware_Slots\gui\imagesets\cyberware.imageset"};
			};
		};
	};
};
class CfgVehicles
{
	class Man;
		class InventoryEquipment
		{
			playerSlots[]+=
			{
				"Cyberware_Frontal_Cortex",
				"Cyberware_Operating_System",
				"Cyberware_Optics",
				"Cyberware_Weapon",
				"Cyberware_Skeleton",
				"Cyberware_Hands",
				"Cyberware_Nervous_System",
				"Cyberware_Circulatory_System",
				"Cyberware_Integumentary_System",
				"Cyberware_Legs"
			};
		};
	};
};
class CfgSlots
{
	class Cyberware_Frontal_Cortex
	{
		name = "Cyberware: Frontal Cortex";
		displayName = "Cyberware: Frontal Cortex";
		ghostIcon = "set:cyberware image:1";
	};
	class Cyberware_Operating_System
	{
		name = "Cyberware: Operating System";
		displayName = "Cyberware: Operating System";
		ghostIcon = "set:cyberware image:2";
	};
	class Cyberware_Optics
	{
		name = "Cyberware: Optics";
		displayName = "Cyberware: Optics";
		ghostIcon = "set:cyberware image:3";
	};
	class Cyberware_Weapon
	{
		name = "Cyberware: Weapon";
		displayName = "Cyberware: Weapon";
		ghostIcon = "set:cyberware image:4";
	};
	class Cyberware_Skeleton
	{
		name = "Cyberware: Skeleton";
		displayName = "Cyberware: Skeleton";
		ghostIcon = "set:cyberware image:5";
	};
	class Cyberware_Hands
	{
		name = "Cyberware: Hands";
		displayName = "Cyberware: Hands";
		ghostIcon = "set:cyberware image:6";
	};
	class Cyberware_Nervous_System
	{
		name = "Cyberware: Nervous System";
		displayName = "Cyberware: Nervous System";
		ghostIcon = "set:cyberware image:7";
	};
	class Cyberware_Circulatory_System
	{
		name = "Cyberware: Circulatory System";
		displayName = "Cyberware: Circulatory System";
		ghostIcon = "set:cyberware image:8";
	};
	class Cyberware_Integumentary_System
	{
		name = "Cyberware: Integumentary System";
		displayName = "Cyberware: Integumentary System";
		ghostIcon = "set:cyberware image:9";
	};
	class Cyberware_Legs
	{
		name = "Cyberware: Legs";
		displayName = "Cyberware: Legs";
		ghostIcon = "set:cyberware image:10";
	};
};

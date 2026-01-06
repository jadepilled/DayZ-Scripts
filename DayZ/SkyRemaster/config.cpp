class CfgPatches
{
    class SkyRemaster
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data","DZ_Scripts","DZ_Worlds_Chernarusplus","DZ_Worlds_Enoch"};
    };
};

class CfgMods
{
    class SkyRemaster
    {
        dir = "SkyRemaster";
        picture = "";
        action = "";
        hideName = 0;
        hidePicture = 0;
        name = "Sky Remaster";
        credits = "";
        author = "";
        authorID = "";
        version = "1.0";
        extra = 0;
        type = "mod";

        dependencies[] = {"World"};
    };
};

class CfgWorlds
{
    class DefaultWorld;
    class CAWorld : DefaultWorld
    {
        // Replace the default color-based sky texture with a custom image map.
        skyTexture = "SkyRemaster\\data\\tex\\sky_co.paa";
        skyTextureR = "SkyRemaster\\data\\tex\\sky_co.paa";
    };
    class ChernarusPlus : CAWorld
    {
        // Ensure the override is applied even if the world redefines the sky textures.
        skyTexture = "SkyRemaster\\data\\tex\\sky_co.paa";
        skyTextureR = "SkyRemaster\\data\\tex\\sky_co.paa";
    };
    class Enoch : CAWorld
    {
        // Livonia inherits CAWorld as well; explicitly restate the custom sky texture.
        skyTexture = "SkyRemaster\\data\\tex\\sky_co.paa";
        skyTextureR = "SkyRemaster\\data\\tex\\sky_co.paa";
    };
};

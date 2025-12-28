/**
 * DayZExpansion_HardlineTiers_Constants.c
 */

//! Folder paths
static const string EXPANSION_HARDLINE_TIER_SETTINGS = EXPANSION_MISSION_SETTINGS_FOLDER + "HardlineTierSettings.json";

//! Tier color configuration
class ExpansionHardlineItemTierColor
{
    static const int Basic = ARGB(255, 157, 157, 157);
    static const int Improved = ARGB(255, 0, 112, 221);
    static const int Advanced = ARGB(255, 163, 53, 238);
    static const int Elite = ARGB(255, 255, 128, 0);
    static const int Prototype = ARGB(255, 199, 38, 81);
    static const int Legendary = ARGB(255, 241, 196, 15);
}

//! Tier text configuration
class ExpansionHardlineItemTierText
{
    static const string Basic = "Basic";
    static const string Improved = "Improved";
    static const string Advanced = "Advanced";
    static const string Elite = "Elite";
    static const string Prototype = "Prototype";
    static const string Legendary = "Legendary";
}

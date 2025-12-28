# Hardline tier addon (separate mod)

You can add new Hardline item tiers without repacking Expansion Hardline by shipping a small addon mod that sits after `@Expansion` in your server load order. The addon only needs a `CfgMods` entry and a scripts PBO that overrides the Hardline enum and default settings.

## Minimal `config.cpp`
```cpp
class CfgPatches
{
    class DZ_Expansion_Hardline_Addon
    {
        requiredAddons[] = {"DZ_Expansion_Hardline"};
        units[] = {};
        weapons[] = {};
    };
};

class CfgMods
{
    class Expansion_Hardline_Addon
    {
        dir = "Expansion_Hardline_Addon";
        name = "Expansion Hardline Addon";
        author = "YourName";
        dependencies[] = {"Game"};

        class defs
        {
            class gameScriptModule
            {
                files[] = {"Expansion/HardlineAddon/scripts/3_Game"};
            };
        };
    };
};
```

Load this mod after the base Expansion mod (`-mod=@Expansion;@ExpansionHardlineAddon`). The `requiredAddons` entry ensures the original Hardline scripts are compiled before the addon.

## Overriding the enum and defaults
Place a script file at `Expansion/HardlineAddon/scripts/3_Game/DayZExpansion_Hardline_Enums.c` to extend the item rarity list. Re-declare the enum with the full set of values you want available:

```c
// Expansion/HardlineAddon/scripts/3_Game/DayZExpansion_Hardline_Enums.c

//! @note Rarity is encoded in 4 bits in the market module, so keep the total at or below 16.
enum ExpansionHardlineItemRarity
{
    NONE,
    Poor,
    Common,
    Uncommon,
    Rare,
    Epic,
    Legendary,
    Mythic,
    Exotic,
    Quest,
    Collectable,
    Ingredient,
    Divine,
    Relic,
    // Add your custom tiers here
    Ancient,
    Celestial
};
```

Because the addon file is compiled after the base Hardline script, the enum is replaced at compile time without touching the original PBO.

## Updating settings defaults
If you need new default reputation thresholds for the added tiers, add a `modded` version of the settings class in `Expansion/HardlineAddon/scripts/3_Game/DayZExpansion_Hardline/Settings/Hardline/ExpansionHardlineSettings.c`:

```c
// Expansion/HardlineAddon/scripts/3_Game/DayZExpansion_Hardline/Settings/Hardline/ExpansionHardlineSettings.c

modded class ExpansionHardlineSettings
{
    static const int VERSION = 12; // keep in sync with the base class until you add persistence fields

    override void Defaults()
    {
        super.Defaults();

        // Shift reputation thresholds to suit the new tiers
        DivineItemRequirement = 14000;
        RelicItemRequirement = 18000;
        AncientItemRequirement = 22000;   // new tier
        CelestialItemRequirement = 26000; // new tier
    }
};
```

Add any new requirement members to the class if you also want them persisted; otherwise, you can rely on the rarity enum and configure item-specific rarities via JSON as usual.

## Notes
- This approach keeps the base Hardline PBO untouched, so updates to Expansion are simpler—your addon just needs to keep the enum and settings in sync with upstream changes.
- Remember the 16-tier limit noted in Hardline: the market module encodes rarity in 4 bits, so exceeding 16 total values will break market rarity support.
- If you share the addon, give it a unique folder/mod name to avoid conflicts with other custom Hardline extensions.

## Can an item have multiple rarities?
Not with the current Hardline implementation. Each class name maps to a single `ExpansionHardlineItemRarity` enum entry via the `ItemRarity` dictionary, and lookups return only that one value for reputation/market logic.【F:Hardline/hardline_scripts/3_Game/DayZExpansion_Hardline/Settings/Hardline/ExpansionHardlineSettings.c†L2511-L2539】

The rarity itself is also encoded as a 4‑bit value in the market module, which assumes one rarity ID per item; supporting two simultaneous rarities would require widening that storage and refactoring all serialization/UI code that reads it.【F:Hardline/hardline_scripts/3_Game/DayZExpansion_Hardline/DayZExpansion_Hardline_Enums.c†L13-L29】

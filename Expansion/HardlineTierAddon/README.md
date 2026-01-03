# Expansion Hardline Tier Addon

This addon layers on top of Expansion Hardline to give every item a **tier level** (0–6) in addition to its existing rarity. Tiers are synced to clients and shown anywhere the Hardline rarity badge appears (inventory tooltip, inspect menu, and the scripted inspection view).

## How it works
- A new network-synced field on `ItemBase` stores the tier level.
- The tier defaults to the item rarity clamped to 0–6, but you can override specific class names in `ExpansionHardlineTierAddon.s_ItemTierOverrides`.
- Tooltips automatically spawn a second badge next to the rarity entry that reads `Tier X`.
- Inspection panels add a white `Tier X` stat line so players can see the level alongside other Hardline details.

## Customizing tiers
The addon now reads `HardlineTierSettings.json` from a **separate profile folder** so it does not touch the core Expansion Hardline settings. On first run it will create:

```
profile/HardlineTiers/HardlineTierSettings.json
```

Update that file on the server to add or adjust tiers, change the badge background color, or tweak the tier text color without rebuilding the mod:

```json
{
    "m_Version": 3,
    "ItemTiers": {
        "m4a1": 6,
        "huntingknife": 2,
        "platecarriervest": 5
    },
    "TierBackgroundColors": {
        "1": 4286611584,
        "2": 4278255360,
        "3": 4278222851,
        "4": 4280619998,
        "5": 4294934784,
        "6": 4294967040
    },
    "TierTextColors": {
        "1": 4278190080,
        "2": 4278190080,
        "3": 4294967295,
        "4": 4294967295,
        "5": 4278190080,
        "6": 4278190080
    }
}
```

- Class names are matched case-insensitively.
- Colors are ARGB integers; you can also use the vanilla/Expansion ARGB helpers if you generate the file manually.
- The file is created with sample entries on first run if it does not already exist.

## Load order
Include the addon after `@Expansion`/`@ExpansionHardline` so the base Hardline types load first:

```
-mod=@Expansion;@ExpansionHardlineTierAddon
```

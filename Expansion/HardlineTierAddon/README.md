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
    "m_Version": 2,
    "ItemTiers": {
        "m4a1": 6,
        "huntingknife": 2,
        "platecarriervest": 5
    },
    "TierBackgroundColors": {
        "1": 4287203721,
        "2": 4284835173,
        "3": 4283058762,
        "4": 4281808695,
        "5": 4280427042,
        "6": 4278190080
    },
    "TierTextColors": {
        "1": 4281940281,
        "2": 4281940281,
        "3": 4287532686,
        "4": 4286808963,
        "5": 4284966759,
        "6": 4284966759
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

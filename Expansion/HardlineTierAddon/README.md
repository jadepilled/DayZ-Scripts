# Expansion Hardline Tier Addon

This addon layers on top of Expansion Hardline to give every item a **tier level** (0–6) in addition to its existing rarity. Tiers are synced to clients and shown anywhere the Hardline rarity badge appears (inventory tooltip, inspect menu, and the scripted inspection view).

## How it works
- A new network-synced field on `ItemBase` stores the tier level.
- The tier defaults to the item rarity clamped to 0–6, but you can override specific class names in `ExpansionHardlineTierAddon.s_ItemTierOverrides`.
- Tooltips automatically spawn a second badge next to the rarity entry that reads `Tier X`.
- Inspection panels add a white `Tier X` stat line so players can see the level alongside other Hardline details.

## Customizing tiers
Edit `Expansion/HardlineTierAddon/scripts/3_Game/DayZExpansion_HardlineTier/ExpansionHardlineTierAddon.c` and add entries to `s_ItemTierOverrides`:

```c
s_ItemTierOverrides.Insert("m4a1", 6);
s_ItemTierOverrides.Insert("huntingknife", 2);
```

Class names are matched case-insensitively.

## Load order
Include the addon after `@Expansion`/`@ExpansionHardline` so the base Hardline types load first:

```
-mod=@Expansion;@ExpansionHardlineTierAddon
```

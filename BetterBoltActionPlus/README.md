# BetterBoltActionPlus

## Approach

The goal is to keep the player’s scoped view stable during bolt cycling without affecting reloads, unjamming, or non-bolt weapons. The most reliable place to counter the bolt-cycle camera lift is the aiming model, because it already aggregates camera offsets and weapon sway from animation and input. In vanilla, `DayZPlayerImplementAiming.ProcessAimFilters` computes `pModel.m_fAimYCamOffset`, which the engine uses to adjust camera pitch during ADS. We add a targeted, reversible correction during bolt-action bolt cycling by subtracting the current weapon hands up/down offset (`HumanCommandWeapons.GetAimingHandsOffsetUD()`), which reflects the bolt-cycle animation’s pitch change.

This keeps the scoped view anchored to the initial aim point while the animation plays, without altering other actions. The change is applied only when:

- The player is in ironsights or optics.
- The weapon in hands is a `BoltActionRifle_Base`.
- The currently running weapon action is `WeaponActions.MECHANISM` with type `WeaponActionMechanismTypes.MECHANISM_OPENED`.

## Why this approach

- **Directly addresses the cause**: the camera shift comes from the animation-driven hands offset during the mechanism action. Using that same offset to counter the camera pitch is the most direct and minimal correction available in script.
- **Minimal scope**: only runs for bolt-action rifles, while scoped, and only during the bolt cycling action.
- **Avoids camera matrix rewrites**: earlier attempts to correct the camera transform in `DayZPlayerCameraIronsights` were brittle and did not reliably offset the internal aiming model used for ADS.

## Development logic

1. Preserve the BetterBoltAction behavior that avoids exiting ADS during bolt cycling.
2. Locate the vanilla aiming model that applies camera offsets (see `Scripts/4_world/entities/dayzplayerimplementaiming.c`).
3. Apply a scoped correction to `pModel.m_fAimYCamOffset` using the hands offset from `HumanCommandWeapons`, but only when the bolt-action mechanism animation is active.

## Challenges and difficulties

- There is no explicit “bolt cycle” flag exposed to script; the closest reliable signal is the weapon mechanism action (`WeaponActions.MECHANISM` + `MECHANISM_OPENED`). This is based on vanilla action types for bolt cycling (`CMD_Reload_BoltAction`) and should be validated against specific modded bolt-actions if they customize action types.
- The animation-driven shift is not a simple camera transform; it is applied through the aiming model and hands offsets. Adjusting camera matrices alone did not fully counter the effect.

## Uncertainties

- If a modded bolt-action uses a different action type for cycling (not `MECHANISM_OPENED`), the correction will not apply. In that case, the action type check should be extended to include the weapon’s specific mechanism action type.
- Some optics may override ironsights behavior. The correction uses the generic ADS camera offset path, which should still apply, but it is worth validating with weapon-specific optics overrides.

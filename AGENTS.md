# Agent Guidelines for DayZ Mod Development

- **Respect DayZ syntax and conventions.** Review vanilla examples in the `DayZ/` scripts for reference before adding or modifying code to ensure correct syntax and idioms.
- **Create mods in their own folders.** When implementing new features or items, prefer adding a new mod directory (e.g., `MyNewMod/`) rather than editing existing mod folders or any vanilla DayZ scripts. Avoid altering base game files unless absolutely required.
- **Leverage existing examples.** If unsure about behavior, class structure, or configuration patterns, consult the original DayZ script folder (`DayZ/`) and existing mods for guidance. Aim for logical, minimal, and efficient implementations that align with established patterns.
- **Maintain logical soundness.** Choose implementations that minimize side effects, keep changes scoped to the new mod, and avoid unnecessary overrides. Validate assumptions against vanilla code where possible.
- **Configuration and assets.** Place new assets (textures, sounds, configs) inside the new mod folder and wire them through that mod’s `config.cpp` and scripts without modifying vanilla assets.
- **PR/commit hygiene.** Follow repository instructions for committing and submitting PRs as requested by the user.

# Sky Remaster

Sky Remaster replaces the vanilla sky dome color texture with a custom image-based texture so modders can ship a detailed sky panorama. The mod uses the SkyDome API (see `1_core/proto/proto.c`) to load a dedicated preset from `SkyRemaster/data/sky/sky_presets.xml` at mission start, initialize the sky system with it, and then patch `CfgWorlds` for CAWorld derivatives (e.g. Chernarus and Livonia) to keep the color layer pointing at the new texture.

Add your final `.paa` sky texture to `SkyRemaster/data/tex/sky_co.paa` before packaging the mod. The placeholder file included here is only to preserve the expected path in source control.

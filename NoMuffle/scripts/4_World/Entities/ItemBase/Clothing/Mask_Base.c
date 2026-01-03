modded class Mask_Base : Clothing
{
	override void HandleVoiceEffect(bool enable, PlayerBase player)
	{
		#ifdef SERVER
		if (player)
		{
			GetGame().SetVoiceEffect(player, VoiceEffectMumbling, false);
			GetGame().SetVoiceEffect(player, VoiceEffectObstruction, false);
		}
		#endif
	}

	override bool IsObstructingVoice()
	{
		return false;
	}

	override int GetVoiceEffect()
	{
		return 0;
	}
}

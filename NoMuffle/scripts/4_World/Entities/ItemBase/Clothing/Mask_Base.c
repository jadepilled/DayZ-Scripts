modded class Mask_Base : Clothing
{
	override void HandleVoiceEffect(bool enable, PlayerBase player)
	{
		if (player)
		{
			#ifdef SERVER
			GetGame().SetVoiceEffect(player, VoiceEffectMumbling, false);
			GetGame().SetVoiceEffect(player, VoiceEffectObstruction, false);
			#endif
		}
	}

	override void MutePlayer(PlayerBase player, bool state)
	{
		if (GetGame() && player)
		{
			#ifdef SERVER
			GetGame().SetVoiceEffect(player, VoiceEffectMumbling, false);
			GetGame().SetVoiceEffect(player, VoiceEffectObstruction, false);
			#endif
		}
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

modded class DayZPlayerImplementAiming
{
	// Based on DayZ/4_world/entities/dayzplayerimplementaiming.c : DayZPlayerImplementAiming.ProcessAimFilters
	override bool ProcessAimFilters(float pDt, SDayZPlayerAimingModel pModel, int stance_index)
	{
		bool result = super.ProcessAimFilters(pDt, pModel, stance_index);

		if (!result)
			return result;

		if (!m_PlayerDpi || !m_PlayerPb)
			return result;

		if (!m_PlayerDpi.IsInIronsights() && !m_PlayerDpi.IsInOptics())
			return result;

		Weapon_Base weapon = Weapon_Base.Cast(m_PlayerPb.GetHumanInventory().GetEntityInHands());
		if (!weapon || !weapon.IsInherited(BoltActionRifle_Base))
			return result;

		HumanCommandWeapons hcw = m_PlayerDpi.GetCommandModifier_Weapons();
		if (!hcw)
			return result;

		if (hcw.GetRunningAction() != WeaponActions.MECHANISM)
			return result;

		if (hcw.GetRunningActionType() != WeaponActionMechanismTypes.MECHANISM_OPENED)
			return result;

		float aimHandsOffsetUD = hcw.GetAimingHandsOffsetUD();
		if (Math.AbsFloat(aimHandsOffsetUD) <= 0.001)
			return result;

		pModel.m_fAimYCamOffset -= aimHandsOffsetUD;

		return result;
	}
}

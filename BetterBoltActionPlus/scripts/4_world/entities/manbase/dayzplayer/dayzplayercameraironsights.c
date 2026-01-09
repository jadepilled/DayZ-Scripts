modded class DayZPlayerCameraIronsights
{
	protected bool m_BoltCycleCompActive;
	protected vector m_BoltCycleBaselinePos;

	// Based on DayZ/Scripts/4_world/entities/manbase/dayzplayer/dayzplayercameraironsights.c OnUpdate; adjusted for bolt cycling camera compensation.
	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		super.OnUpdate(pDt, pOutResult);

		PlayerBase player = PlayerBase.Cast(m_pPlayer);
		bool isBoltCycling = false;
		if (player)
		{
			WeaponManager weapon_manager = player.GetWeaponManager();
			if (weapon_manager && weapon_manager.IsBoltActionCycling())
			{
				isBoltCycling = true;
			}
		}

		if (isBoltCycling)
		{
			if (!m_BoltCycleCompActive)
			{
				m_BoltCycleBaselinePos = pOutResult.m_CameraTM[3];
				m_BoltCycleCompActive = true;
			}

			pOutResult.m_CameraTM[3][1] = m_BoltCycleBaselinePos[1];
		}
		else
		{
			m_BoltCycleCompActive = false;
		}
	}
}

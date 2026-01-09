modded class DayZPlayerCameraIronsights
{
	// Based on DayZ/4_world/entities/manbase/dayzplayer/dayzplayercameraironsights.c : DayZPlayerCameraIronsights.OnUpdate
	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		super.OnUpdate(pDt, pOutResult);

		if (!m_weaponUsed || !m_weaponUsed.IsInherited(BoltActionRifle_Base))
			return;

		if (!m_CommandWeapons)
			return;

		if (m_CommandWeapons.GetRunningAction() != WeaponActions.MECHANISM)
			return;

		if (m_CommandWeapons.GetRunningActionType() != WeaponActionMechanismTypes.MECHANISM_OPENED)
			return;

		float aimHandsOffsetUD = m_CommandWeapons.GetAimingHandsOffsetUD();
		if (Math.AbsFloat(aimHandsOffsetUD) <= 0.001)
			return;

		vector correction[3];
		vector baseTM[3];
		vector resultTM[3];
		Math3D.YawPitchRollMatrix(Vector(0, -aimHandsOffsetUD, 0), correction);
		baseTM[0] = pOutResult.m_CameraTM[0];
		baseTM[1] = pOutResult.m_CameraTM[1];
		baseTM[2] = pOutResult.m_CameraTM[2];
		Math3D.MatrixMultiply3(baseTM, correction, resultTM);
		pOutResult.m_CameraTM[0] = resultTM[0];
		pOutResult.m_CameraTM[1] = resultTM[1];
		pOutResult.m_CameraTM[2] = resultTM[2];
	}
}

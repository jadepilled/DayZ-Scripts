modded class WeaponManager
{
	protected bool ShouldExitSightsOnWeaponAction()
	{
		if (m_PendingWeaponAction == AT_WPN_EJECT_BULLET && m_WeaponInHand && m_WeaponInHand.IsInherited(BoltActionRifle_Base))
			return false;

		return true;
	}

	// Based on DayZ/4_world/classes/weapons/weaponmanager.c Update; adjusted to preserve ADS during bolt cycling.
	override void Update(float deltaT)
	{
		if (m_WeaponInHand != m_player.GetItemInHands())
		{
			if (m_WeaponInHand)
			{
				m_SuitableMagazines.Clear();
				OnWeaponActionEnd();
			}
			m_WeaponInHand = Weapon_Base.Cast(m_player.GetItemInHands());
			if (m_WeaponInHand)
			{
				m_MagazineInHand = null;
				//SET new magazine
				SetSutableMagazines();
				m_WeaponInHand.SetSyncJammingChance(0);
			}
			m_AnimationRefreshCooldown = 0;
		}
		
		if (m_WeaponInHand)
		{
			if(m_AnimationRefreshCooldown)
			{
				m_AnimationRefreshCooldown--;
			
				if( m_AnimationRefreshCooldown == 0)
				{
					RefreshAnimationState();
				}
			}
		
			if (!GetGame().IsMultiplayer())
			{
				m_WeaponInHand.SetSyncJammingChance(m_WeaponInHand.GetChanceToJam());
			}
			else
			{
				if ( m_NewJamChance >= 0)
				{
					m_WeaponInHand.SetSyncJammingChance(m_NewJamChance);
					m_NewJamChance = -1;
					m_WaitToSyncJamChance = false;
				}
				if (GetGame().IsServer() && !m_WaitToSyncJamChance )
				{
					float actual_chance_to_jam;
					actual_chance_to_jam = m_WeaponInHand.GetChanceToJam();
					if ( Math.AbsFloat(m_WeaponInHand.GetSyncChanceToJam() - m_WeaponInHand.GetChanceToJam()) > 0.001 )
					{
						DayZPlayerSyncJunctures.SendWeaponJamChance(m_player, m_WeaponInHand.GetChanceToJam());
						m_WaitToSyncJamChance = true;
					}
				}
			}
			
			if(m_readyToStart)
			{
				StartPendingAction();
				m_readyToStart = false;
				return;
			}
		
			if( !m_InProgress || !m_IsEventSended )
				return;
		
			if(m_canEnd)
			{
				if(m_WeaponInHand.IsIdle())
				{
					OnWeaponActionEnd();
				}
				else if(m_justStart)
				{
					m_InIronSight = m_player.IsInIronsights();
					m_InOptic = m_player.IsInOptics();
		
					if(m_InIronSight || m_InOptic)
					{
						//'RequestResetADSSync' can be called here, if ADS reset is desired
						if (ShouldExitSightsOnWeaponAction())
						{
							m_player.ExitSights();
						}
					}
				
					m_justStart = false;
				}
			
			}
			else
			{
				m_canEnd = true;
				m_justStart = true;
			}
		}
		else
		{
			if ( m_MagazineInHand != m_player.GetItemInHands() )
			{
				m_MagazineInHand = MagazineStorage.Cast(m_player.GetItemInHands());
				if ( m_MagazineInHand )
				{
					SetSutableMagazines();
				}
			}
		
		
		}
	}
}

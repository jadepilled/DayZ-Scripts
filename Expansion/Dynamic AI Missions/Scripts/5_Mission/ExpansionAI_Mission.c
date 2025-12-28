/*
Dynamic AI missions for Expansion AI by Touchofdeath Version 2.0

Credits to:
Sid Debian [76561197991259240] for assisting me with this project! (Moving to server-side mod, and fixing markers)
Credits to Chris (CBD Modding) for support on Enfusion Modders! (Cleaning up bad refs)
*/

/*
Update 2.0 patch notes:
- Added support for Expansion Loadouts
- Added support for simultaneous missions
- Added option to enable or disable 2D or 3D markers
- Added option to use different map markers. If using LB Groups you can specify a file path
- Added option to set radius of marker circle if using LB Groups
- Added config parameters for messages and map icons so you can configure your own language
- Added config option to specify if helicopter rewards are allowed for a specific mission, and reward type 3 for helicopters
- Patched issue with alternative CarLock mods not allowing the mod to work
- Unable to add marker support for V++ map
*/

//TO DO IN FUTURE UPDATE:
/// Add option for loot tiers for the rewards?
/// Add quantity and stack count variables in loot table configs

ref ExpansionAI_Mission AIMissions = new ExpansionAI_Mission();

modded class MissionServer
			{    override void OnInit()
	{
        super.OnInit();
        AIMissions.OnInit();
	}
}

class ExpansionAI_Mission
			{	ref array<ref AIM_Missions> AIM_MissionsArray = new ref array<ref AIM_Missions>();

	bool AIM_IsFirstStartup = true;
	bool AIM_Debug = false
	bool AIM_DebugQuickMode = false;
	
	string AIM_NotificationMarkerString;
	string AIM_MissionsTitle;
	string AIM_StartMessage;
	string AIM_WinMessage;
	string AIM_LoseMessage;
	
	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerModule	m_MarkerModuleEvent;
	#endif

	void ExpansionAI_Mission()
	{
		#ifdef EXPANSIONMODNAVIGATION
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModuleEvent);
		#endif

		AIM_NotificationMarkerString = GetAIMConfig().Settings.Get(0).GetNotificationIconString();
		AIM_MissionsTitle = GetAIMConfig().Settings.Get(0).GetMissionsTitle();
		AIM_StartMessage = GetAIMConfig().Settings.Get(0).GetStartMessage();
		AIM_WinMessage = GetAIMConfig().Settings.Get(0).GetWinMessage();
		AIM_LoseMessage = GetAIMConfig().Settings.Get(0).GetLoseMessage();
		
		if(AIM_NotificationMarkerString == "") AIM_NotificationMarkerString = "Soldier";
		
		if(AIM_MissionsTitle == "") AIM_MissionsTitle = "AI Mission";
		if(AIM_StartMessage == "") AIM_StartMessage = "Mission '%1' starting! Eliminate all threats to claim the loot!";
		if(AIM_WinMessage == "") AIM_WinMessage = "Mission '%1' completed! All threats were eliminated!";
		if(AIM_LoseMessage == "") AIM_LoseMessage = "Mission '%1' failed! You ran out of time!";

		Print("[AI Missions] Loaded!");
	}
	
	void ~ExpansionAI_Mission()
	{
		if(this.AIM_MissionsArray) this.AIM_MissionsArray.Clear();
		this.AIM_MissionsArray = NULL;
		#ifdef EXPANSIONMODNAVIGATION
		this.m_MarkerModuleEvent = NULL;
		#endif
		this.AIM_IsFirstStartup = false;
		this.AIM_Debug = false;
		this.AIM_DebugQuickMode = false;
	}
	
	void OnInit()
	{
		if(GetAIMConfig().Settings.Get(0).DebugEnabled())
		{
			AIM_Debug = true;
			
			Print("[AI Missions] DEBUG MODE ENABLED.");
			
				if(GetAIMConfig().Settings.Get(0).DebugQuickmode())
			{
				AIM_DebugQuickMode = true;
				
				//Force quick mode parameters
				GetAIMConfig().Settings.Get(0).DoQuickMode();
				
				Print("[AI Missions] DEBUG QUICK MODE ENABLED.");
			}
			
		}
		
		Print("[AI Missions] Initialized! Waiting for " + GetAIMConfig().Settings.Get(0).GetStartDelay() + " seconds before running cleanup script and attempting to start first mission.");

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(FirstStartAIMission, GetAIMConfig().Settings.Get(0).GetStartDelay() * 1000); //Start a mission after starting delay

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateAIMissions, 1000, true);
	}
	
	void FirstStartAIMission()
	{
		MissionCleanup();
		StartAIMission();
	}
	
	//Function for starting a new AI Mission
	void StartAIMission()
	{
		if(GetAIMConfig().Missions.Count() == 0)
		{
			Print("[AI Missions] NO MISSIONS DEFINED! MISSIONS SCRIPT WILL NOT BE STARTED");
			Print("[AI Missions] NO MISSIONS DEFINED! MISSIONS SCRIPT WILL NOT BE STARTED");
			Print("[AI Missions] NO MISSIONS DEFINED! MISSIONS SCRIPT WILL NOT BE STARTED");
			Print("[AI Missions] NO MISSIONS DEFINED! MISSIONS SCRIPT WILL NOT BE STARTED");
			Print("[AI Missions] NO MISSIONS DEFINED! MISSIONS SCRIPT WILL NOT BE STARTED");
			
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(StartAIMission);
			
			return;
		}
		
		int ChosenNewMissionID = Math.RandomInt(0,GetAIMConfig().Missions.Count());
		
		if(AIM_Debug) Print("[AI Missions] DEBUG - Attempting to start new AI Mission ID '" + ChosenNewMissionID + "'."); //Debug

		if(!CanStartAIMission(ChosenNewMissionID))
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Failed to start new Mission ID '" + ChosenNewMissionID + "'."); //Debug
			
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartAIMission, GetAIMConfig().Settings.Get(0).GetFailStartDelay() * 1000, false); //Retry starting AI mission

			return;
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(StartAIMission);

		if(AIM_Debug) Print("[AI Missions] DEBUG - Setting up default parameters for new Mission ID '" + ChosenNewMissionID + "'."); //Debug
		
		int rewardID = PickMissionReward(ChosenNewMissionID);
		int botAmount = Math.RandomInt(GetAIMConfig().Missions.Get(ChosenNewMissionID).GetMinAICount(),GetAIMConfig().Missions.Get(ChosenNewMissionID).GetMaxAICount());

		AIM_MissionsArray.Insert(new AIM_Missions(ChosenNewMissionID, rewardID, botAmount));
		
		Print("[AI Missions] Mission " + GetAIMConfig().Missions.Get(ChosenNewMissionID).GetName() + " starting.");
		if(AIM_Debug) Print("[AI Missions] DEBUG - [ID: " + ChosenNewMissionID + " | BOTS: " + botAmount + " | REWARD ID: " + rewardID + "]"); //Debug
		
		if(!GetAIMConfig().Settings.Get(0).DebugNotificationsDisabled()) ExpansionNotification(AIM_MissionsTitle, string.Format(AIM_StartMessage, GetAIMConfig().Missions.Get(ChosenNewMissionID).GetName()), AIM_NotificationMarkerString, COLOR_EXPANSION_NOTIFICATION_MISSION, GetAIMConfig().Settings.Get(0).GetNotificationTime()).Create();
		
		int NewMissionID = AIM_MissionsArray.Count() - 1;
		
		if(!GetAIMConfig().Settings.Get(0).DebugMarkersDisabled())
		{
			string AIM_MarkerString = GetAIMConfig().Settings.Get(0).GetMarkerString();
			
			bool AIM_Marker2D = GetAIMConfig().Settings.Get(0).GetMarker2D();
			bool AIM_Marker3D = GetAIMConfig().Settings.Get(0).GetMarker3D();
			
			string AIM_MarkerFilePath = GetAIMConfig().Settings.Get(0).GetMarkerFilePath();

			int AIM_MarkerRadius = GetAIMConfig().Settings.Get(0).GetMarkerRadius();
			int AIM_MarkerAlpha = GetAIMConfig().Settings.Get(0).GetMarkerAlpha();
			int AIM_MarkerRed = GetAIMConfig().Settings.Get(0).GetMarkerRed();
			int AIM_MarkerGreen = GetAIMConfig().Settings.Get(0).GetMarkerGreen();
			int AIM_MarkerBlue = GetAIMConfig().Settings.Get(0).GetMarkerBlue();
			
				if(AIM_MarkerRadius < 0) AIM_MarkerRadius = 0;
				if(AIM_MarkerAlpha < 0) AIM_MarkerAlpha = 0;
			else if(AIM_MarkerAlpha > 255) AIM_MarkerAlpha = 255;
				if(AIM_MarkerRed < 0) AIM_MarkerRed = 0;
			else if(AIM_MarkerRed > 255) AIM_MarkerRed = 255;
				if(AIM_MarkerGreen < 0) AIM_MarkerGreen = 0;
			else if(AIM_MarkerGreen > 255) AIM_MarkerGreen = 255;
				if(AIM_MarkerBlue < 0) AIM_MarkerBlue = 0;
			else if(AIM_MarkerBlue > 255) AIM_MarkerBlue = 255;
			
			#ifdef LBmaster_Groups
			Print("[AI Missions] DEBUG - Detected LBMaster Groups for marker."); //Debug
			
				if(AIM_MarkerString == "") AIM_MarkerString = "LBmaster_Groups\\gui\\icons\\player.paa";
			
			LBServerMarker AIM_SVMarker;
			
			#ifdef LBmaster_Rework
			AIM_SVMarker = LBStaticMarkerManager.Get.AddTempServerMarker(AIM_MissionsTitle, GetAIMConfig().Missions.Get(ChosenNewMissionID).GetPosition(), AIM_MarkerFilePath, ARGB(AIM_MarkerAlpha, AIM_MarkerRed, AIM_MarkerGreen, AIM_MarkerBlue), false, AIM_Marker3D, AIM_Marker2D, AIM_Marker2D);
			#endif
			#ifndef LBmaster_Rework
			AIM_SVMarker = LBStaticMarkerManager.Get().AddTempServerMarker(AIM_MissionsTitle, GetAIMConfig().Missions.Get(ChosenNewMissionID).GetPosition(), AIM_MarkerFilePath, ARGB(AIM_MarkerAlpha, AIM_MarkerRed, AIM_MarkerGreen, AIM_MarkerBlue), false, AIM_Marker3D, AIM_Marker2D, AIM_Marker2D);
			#endif
			AIM_SVMarker.SetRadius(AIM_MarkerRadius, ARGB(AIM_MarkerAlpha, AIM_MarkerRed, AIM_MarkerGreen, AIM_MarkerBlue), true);
			AIM_MissionsArray.Get(NewMissionID).SetLBMarkerID(AIM_SVMarker.uid);
			#else
			#ifdef EXPANSIONMODNAVIGATION
			Print("[AI Missions] DEBUG - Detected Expansion Navigation for marker."); //Debug
			
				if(AIM_MarkerString == "") AIM_MarkerString = "Soldier";
			
			vector misionPos = GetAIMConfig().Missions.Get(ChosenNewMissionID).GetPosition();
			
			AIM_MissionsArray.Get(NewMissionID).SetExpansionMarkerData(m_MarkerModuleEvent.CreateServerMarker(AIM_MissionsTitle, AIM_MarkerString, misionPos, ARGB(AIM_MarkerAlpha, AIM_MarkerRed, AIM_MarkerGreen, AIM_MarkerBlue), AIM_Marker3D));
			#endif
			#endif
		}
		
		MissionRewardInit(NewMissionID, ChosenNewMissionID, rewardID);
		MissionObjectsInit(NewMissionID);
		
		if(AIM_Debug) Print("[AI Missions] DEBUG - Successfully initialized new mission!"); //Debug

		if(AIM_MissionsArray.Count() < GetAIMConfig().Settings.Get(0).GetMaxSimultaneousMissions())
		{
			Print("[AI Missions] Countdown before next simultaneous mission: " + GetAIMConfig().Settings.Get(0).GetStartDelay() + " seconds.");

			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartAIMission, GetAIMConfig().Settings.Get(0).GetStartDelay() * 1000, true);
		}
	}
	
	//Function to check if it's safe to start a new proposed mission ID
	bool CanStartAIMission(int proposedmissionID)
	{
		if(!AIM_Missions)
		{
			Print("[AI Missions] [ERROR] Missions array not initialized, unable to start mission. Retrying in " + GetAIMConfig().Settings.Get(0).GetFailStartDelay() + " seconds.");

			return false // Missions array not initialized
		}
		
		if(AIM_MissionsArray.Count() >= GetAIMConfig().Settings.Get(0).GetMaxSimultaneousMissions())
		{
			Print("[AI Missions] Maximum number of simultaneous missions already reached. Retrying in " + GetAIMConfig().Settings.Get(0).GetFailStartDelay() + " seconds.");

			return false; // Already running desired quantity of missions
		}
		
		for(int i=0; i<AIM_MissionsArray.Count(); i++)
		{
				if(AIM_MissionsArray.Get(i).GetMissionID() == proposedmissionID)
			{
				Print("[AI Missions] Missions with this ID is already in progress. Retrying in " + GetAIMConfig().Settings.Get(0).GetFailStartDelay() + " seconds.");

				return false; // Already running desired quantity of missions
			}
		}
		
		if(GetAIMConfig().Missions.Get(proposedmissionID).GetName() == "")
		{
				if(AIM_IsFirstStartup) Print("[AI Missions] Mission name undefined, unable to start mission. Retrying in " + GetAIMConfig().Settings.Get(0).GetStartDelay() + " seconds.");
			else Print("[AI Missions] Mission name undefined, unable to start mission. Retrying in " + GetAIMConfig().Settings.Get(0).GetFailStartDelay() + " seconds.");

			return false;
		}

		if(GetGame().GetFps() > 2) // Server FPS too low
		{
				if(AIM_IsFirstStartup) Print("[AI Missions] Unable to start new mission as server FPS is too low. Retrying in " + GetAIMConfig().Settings.Get(0).GetStartDelay() + " seconds.");
			else Print("[AI Missions] Unable to start new mission as server FPS is too low. Retrying in " + GetAIMConfig().Settings.Get(0).GetFailStartDelay() + " seconds.");

			return false;
		}
		
		array<Man> all_players = new array<Man>;
		GetGame().GetPlayers(all_players);

		if(all_players.Count() < GetAIMConfig().Settings.Get(0).GetMinPlayers()) // Not enough players
		{
				if(AIM_IsFirstStartup) Print("[AI Missions] Not enough players to start mission. Retrying in " + GetAIMConfig().Settings.Get(0).GetStartDelay() + " seconds.");
			else Print("[AI Missions] Not enough players to start mission. Retrying in " + GetAIMConfig().Settings.Get(0).GetFailStartDelay() + " seconds.");

			return false;
		}

		for(int j=0; j<all_players.Count(); j++) // Player in spawn area check
		{
			PlayerBase player = PlayerBase.Cast(all_players.Get(j));
				if(player)
			{
				if(vector.Distance(GetAIMConfig().Missions.Get(proposedmissionID).GetPosition(), player.GetPosition()) <= (GetAIMConfig().Missions.Get(proposedmissionID).GetAIRange() + 200))
				{
					if(AIM_IsFirstStartup) Print("[AI Missions] Player in spawn area, unable to start mission. Retrying in " + GetAIMConfig().Settings.Get(0).GetStartDelay() + " seconds.");
					else Print("[AI Missions] Player in spawn area, unable to start mission. Retrying in " + GetAIMConfig().Settings.Get(0).GetFailStartDelay() + " seconds.");

					return false;
				}
			}
		}
		
		return true;
	}
	
	bool IsPlayerInRadius(vector m_Position, int m_Radius)
	{
		array<Man> all_players = new array<Man>;
		GetGame().GetPlayers(all_players);
		if(all_players.Count() > 0)
		{
			//Check for players in mission radius, or spawn radius for bots
			for(int i=0; i<all_players.Count(); i++)
			{
				PlayerBase player = PlayerBase.Cast(all_players.Get(i));
				
				if(player)
				{
					if(vector.Distance(m_Position, player.GetPosition()) <= m_Radius)
					{
						return true;
					}
				}
			}	
		}
		
		return false;
	}

	//Mission monitoring script that runs once per second while mission is active
	void UpdateAIMissions()
	{
		if(!AIM_Missions) return; //Missions array not initialized
		
		if(AIM_MissionsArray.Count() == 0) return; //No missions running
		
		int AIM_MissionToRemove = -1;
		
		for(int m=0; m<AIM_MissionsArray.Count(); m++)
		{
				if(AIM_MissionsArray.Get(m).GetIsCleanedUp()) continue;
			
			bool FoundPlayerInInnerRadius = false;
			bool FoundPlayerInSpawnRadius = false;
			bool IsPendingFinish = AIM_MissionsArray.Get(m).AIM_MissionPendingFinish;
			
			int MissionID = AIM_MissionsArray.Get(m).GetMissionID();
			int MissionRewardID = AIM_MissionsArray.Get(m).GetMissionRewardID();
			int CurrentMissionTime = AIM_MissionsArray.Get(m).GetCurrentMissionTime();
			int CurrentMissionExtensionTime = AIM_MissionsArray.Get(m).GetCurrentMissionTimeExtension();
			int RemainingBots = AIM_MissionsArray.Get(m).GetRemainingBots();
			int RemainingBotsIn = RemainingBots;
			
			vector MissionPos = GetAIMConfig().Missions.Get(MissionID).GetPosition();
			
			EntityAI TempRewardEAI = EntityAI.Cast(AIM_MissionsArray.Get(m).GetMissionRewardEAI());
		
			eAIBase TempEAIBase;

				if(GetAIMConfig().Settings.Get(0).DebugIgnoreRadius() || AIM_DebugQuickMode) // Debug Modes
			{
				FoundPlayerInInnerRadius = false; //During debug we force mission to ignore any close players so it force despawns.
				FoundPlayerInSpawnRadius = true; //During debug we force AI to spawn no matter if players are close or not.
			}
			else
			{
				if(IsPlayerInRadius(MissionPos, GetAIMConfig().Settings.Get(0).GetInnerRadius())) FoundPlayerInInnerRadius = true;
				if(IsPlayerInRadius(MissionPos, GetAIMConfig().Settings.Get(0).GetSpawnRadius())) FoundPlayerInSpawnRadius = true;
			}
		
				if((AIM_MissionsArray.Get(m).AIM_EAIArray.Count() <= RemainingBots) && FoundPlayerInSpawnRadius && !IsPendingFinish)
			{
				//Spawn missing bots
				for(int k=0; k<(RemainingBots - AIM_MissionsArray.Get(m).AIM_EAIArray.Count()); k++)
				{
					if(Class.CastTo(TempEAIBase, SpawnEAI(MissionID)))
					{
						if(AIM_Debug) Print("[AI Missions] DEBUG - Spawned EAI NPC."); //Debug
						
						AIM_MissionsArray.Get(m).AIM_EAIArray.Insert(TempEAIBase);
						AIM_MissionsArray.Get(m).AIM_EAIWanderTimeArray.Insert(0);
					}
				}
			}

				if(AIM_MissionsArray.Get(m).AIM_EAIArray.Count() > 0)
			{
				array<eAIBase> TempEAIArray = new array<eAIBase>;
				array<int> TempWanderTimeArray = new array<int>;
				
				for(int j=0; j<AIM_MissionsArray.Get(m).AIM_EAIArray.Count(); j++)
				{
					if(Class.CastTo(TempEAIBase, AIM_MissionsArray.Get(m).AIM_EAIArray.Get(j)))
					{
						if(!TempEAIBase.IsAlive())
						{
							if(AIM_Debug) Print("[AI Missions] DEBUG - EAI NPC killed, reducing mission remaining bot count by one."); //Debug
							
							if(!GetAIMConfig().Missions.Get(MissionID).GetAIDropLoot())
							{
								if(AIM_Debug) Print("[AI Missions] DEBUG - Deleting killed AI to remove loot"); //Debug
								TempEAIBase.eAI_Despawn();
							}

							RemainingBots--;
						}
						else if(!FoundPlayerInSpawnRadius)
						{
							if(AIM_Debug) Print("[AI Missions] DEBUG - No players in spawn radius, despawning EAI NPC."); //Debug

							//Remove and despawn bots as no players in radius
							TempEAIBase.eAI_Despawn();
						}
						else
						{
							TempEAIArray.Insert(TempEAIBase); //Player in spawn radius, and bot alive.
							
							int TempWanderTime = AIM_MissionsArray.Get(m).AIM_EAIWanderTimeArray.Get(j);

							if(vector.Distance(GetAIMConfig().Missions.Get(MissionID).GetPosition(), TempEAIBase.GetPosition()) >= GetAIMConfig().Settings.Get(0).GetBotsTimeoutRadius())
							{
								//Bot has wandered outside of normal mission area. Start counting
								
								TempWanderTime += 1;
							}

							if(TempWanderTime >= GetAIMConfig().Settings.Get(0).GetBotsTimeoutTime())
							{
								TempEAIBase.eAI_RemoveTargets(); // De-agro and send back to spawn
								
								eAIGroup aiGroup = TempEAIBase.GetGroup();
								
								if(aiGroup)
								{
									aiGroup.AddWaypoint(GetAIMConfig().Missions.Get(MissionID).GetPosition());
									aiGroup.SetWaypointBehaviourAuto(eAIWaypointBehavior.ONCE);

									if(aiGroup.GetWaypoints().Count() == 1) //New waypoint is the only waypoint, so recalculate!
									{
										TempEAIBase.GetPathFinding().ForceRecalculate(true);
									}
								}
										
								TempWanderTime = 0;
							}

							TempWanderTimeArray.Insert(TempWanderTime);
						}
					}
					else
					{
						if(AIM_Debug) Print("[AI Missions] DEBUG - EAI NPC has been deleted since last update, reducing mission remaining bot count by one."); //Debug

						//Bot was deleted since last update
						RemainingBots--;
					}
				}
				
				AIM_MissionsArray.Get(m).AIM_EAIArray = TempEAIArray;
				AIM_MissionsArray.Get(m).AIM_EAIWanderTimeArray = TempWanderTimeArray;
			}
		
				if((RemainingBots <= 0) && !IsPendingFinish) //Win
			{
				Print("[AI Missions] Mission " + GetAIMConfig().Missions.Get(MissionID).GetName() + " completed.");
				
				if(!GetAIMConfig().Settings.Get(0).DebugNotificationsDisabled()) ExpansionNotification(AIM_MissionsTitle, string.Format(AIM_WinMessage, GetAIMConfig().Missions.Get(MissionID).GetName()), AIM_NotificationMarkerString, COLOR_EXPANSION_NOTIFICATION_MISSION, GetAIMConfig().Settings.Get(0).GetNotificationTime()).Create();

				MissionRewardRelease(TempRewardEAI, AIM_MissionsArray.Get(m).GetMissionRewardID(), MissionID, m);

				CurrentMissionTime = GetAIMConfig().Settings.Get(0).GetTimeLimit();
				
				IsPendingFinish = true;
			}
			else if((CurrentMissionTime >= GetAIMConfig().Settings.Get(0).GetTimeLimit()) && FoundPlayerInSpawnRadius && !IsPendingFinish && (CurrentMissionExtensionTime < GetAIMConfig().Settings.Get(0).GetTimeExtensionMax())) //Ran out of time, but someone is still in mission area trying to finish the mission.
			{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Mission is out of time, but player detected in the area, extending time!"); //Debug

				CurrentMissionExtensionTime += GetAIMConfig().Settings.Get(0).GetTimeExtensionMax();
				CurrentMissionTime -= GetAIMConfig().Settings.Get(0).GetTimeExtensionMax();
			}
			else if((CurrentMissionTime >= GetAIMConfig().Settings.Get(0).GetTimeLimit()) && (!FoundPlayerInSpawnRadius || GetAIMConfig().Settings.Get(0).DebugForceWin()) && !IsPendingFinish) //Ran out of time
			{
				if(!GetAIMConfig().Settings.Get(0).DebugNotificationsDisabled()) ExpansionNotification(AIM_MissionsTitle, string.Format(AIM_LoseMessage, GetAIMConfig().Missions.Get(MissionID).GetName()), AIM_NotificationMarkerString, COLOR_EXPANSION_NOTIFICATION_MISSION, GetAIMConfig().Settings.Get(0).GetNotificationTime()).Create();

				if(GetAIMConfig().Settings.Get(0).DebugForceWin()) 
				{
					Print("[AI Missions] DEBUG - Mission " + GetAIMConfig().Missions.Get(MissionID).GetName() + " completed.");

					MissionRewardRelease(TempRewardEAI, AIM_MissionsArray.Get(m).GetMissionRewardID(), MissionID, m);

					CurrentMissionTime = GetAIMConfig().Settings.Get(0).GetTimeLimit();
					
					IsPendingFinish = true;
				}
				else
				{
					Print("[AI Missions] Mission " + GetAIMConfig().Missions.Get(MissionID).GetName() + " ran out of time, despawned.");

					if(TempRewardEAI && (GetAIMConfig().RewardObjects.Get(MissionRewardID).GetRewardType() == 2))
					{
						TempRewardEAI.SetHealth(0.0); //Destroy vehicles so cheeky players cant come in and repair them
					}
				}


				if(AIM_MissionsArray.Get(m).AIM_EAIArray.Count() > 0)
				{
					if(AIM_Debug) Print("[AI Missions] DEBUG - Now despawning all bots."); //Debug

					for(int l=0; l<AIM_MissionsArray.Get(m).AIM_EAIArray.Count(); l++)
					{
						if(Class.CastTo(TempEAIBase, AIM_MissionsArray.Get(m).AIM_EAIArray.Get(l)))
						{
							if(AIM_Debug) Print("[AI Missions] DEBUG - Found and despawning a bot!"); //Debug

							TempEAIBase.eAI_Despawn();
						}
					}
					AIM_MissionsArray.Get(m).AIM_EAIArray = new array<eAIBase>;
				}

				IsPendingFinish = true;
			}
		
				if((AIM_MissionToRemove == -1) && IsPendingFinish && (!FoundPlayerInInnerRadius || (CurrentMissionTime >= (GetAIMConfig().Settings.Get(0).GetTimeLimit() + GetAIMConfig().Settings.Get(0).GetCleanupTimeRunning()))))
			{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Mission is over, and safe to pre-clean up!"); //Debug
				
				//Remove Marker
				#ifdef LBmaster_Groups
				if(AIM_Debug) Print("[AI Missions] DEBUG - Removing LB Marker!"); //Debug

				#ifdef LBmaster_Rework
				LBStaticMarkerManager.Get.RemoveServerMarker(AIM_MissionsArray.Get(m).GetLBMarkerID());
				#endif
				#ifndef LBmaster_Rework
				LBStaticMarkerManager.Get().RemoveServerMarker(AIM_MissionsArray.Get(m).GetLBMarkerID());
				#endif

				#else
				#ifdef EXPANSIONMODNAVIGATION
				if(AIM_Debug) Print("[AI Missions] DEBUG - Removing Expansion Navigation Marker!"); //Debug
				if(AIM_MissionsArray.Get(m).GetExpansionMarkerData())
				{
					if(m_MarkerModuleEvent)
					{
						m_MarkerModuleEvent.RemoveServerMarker(AIM_MissionsArray.Get(m).GetExpansionMarkerData().GetUID());
					}
					AIM_MissionsArray.Get(m).SetExpansionMarkerData(NULL);
				}
				#endif
				#endif
				if(AIM_Debug) Print("[AI Missions] DEBUG - End of mission pre clean-up successful!"); //Debug

				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartAIMission, GetAIMConfig().Settings.Get(0).GetTimeBetweenMissions() * 1000, true); //Start a mission after starting delay
				
				MissionCleanup(m);
				
				AIM_MissionToRemove = m;
			}
			
			CurrentMissionTime += 1;
			
				if(RemainingBotsIn != RemainingBots) AIM_MissionsArray.Get(m).SetRemainingBots(RemainingBots);

			AIM_MissionsArray.Get(m).SetCurrentMissionTime(CurrentMissionTime);
			AIM_MissionsArray.Get(m).SetCurrentMissionTimeExtension(CurrentMissionExtensionTime);
			AIM_MissionsArray.Get(m).SetMissionPendingFinish(IsPendingFinish);

				if(AIM_MissionToRemove != -1)
			{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Removing mission from missions array: " + AIM_MissionToRemove + " (Mission ID: " + AIM_MissionsArray.Get(AIM_MissionToRemove).GetMissionID() + ")"); //Debug
				AIM_MissionsArray.Remove(AIM_MissionToRemove);
				AIM_MissionToRemove = -1;
			}
		}
	}
	
	//Function to spawn the AI
	eAIBase SpawnEAI(int m_MissionID)
	{
		if(AIM_Debug) Print("[AI Missions] DEBUG - SPAWNING NEW EAI NPC"); //Debug
			
		private eAIBase m_ExpansionAINPC;
		ItemBase tempIB;
		EntityAI tempEAI;
		
		vector position = GetAIMConfig().Missions.Get(m_MissionID).GetPosition();
		
		vector botSpawnPos = Vector(position[0] + Math.RandomInt(-5, 5), position[1], position[2] + Math.RandomInt(-5, 5));
		
		if(!Class.CastTo(m_ExpansionAINPC, GetGame().CreateObject(GetRandomAI(), botSpawnPos)))
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Failed to spawn new EAI NPC"); //Debug
			
			return NULL;
		}
		
		m_ExpansionAINPC.GetGroup().SetFaction(new eAIFactionMercenaries());
		m_ExpansionAINPC.eAI_SetUnlimitedReload(true);
		m_ExpansionAINPC.eAI_SetDamageMultiplier(GetAIMConfig().Missions.Get(m_MissionID).GetAIDamageDoneMultiplier());
		m_ExpansionAINPC.eAI_SetDamageReceivedMultiplier(GetAIMConfig().Missions.Get(m_MissionID).GetAIDamageTakenMultiplier());
		m_ExpansionAINPC.eAI_SetDespawnOnLoosingAggro(false);
		m_ExpansionAINPC.SetMovementSpeedLimits(1, 3);
		m_ExpansionAINPC.eAI_SetNoiseInvestigationDistanceLimit(900);
		m_ExpansionAINPC.eAI_SetSniperProneDistanceThreshold(GetAIMConfig().Missions.Get(m_MissionID).GetAIRange());
		
		if(GetAIMConfig().Settings.Get(0).DebugForceWin())
		{
			m_ExpansionAINPC.eAI_SetAccuracy(0.0, 0.0); //During debug we make bots terrible shots
			m_ExpansionAINPC.eAI_SetThreatDistanceLimit(5); //During debug we make bots not see players
		}
		else
		{
			m_ExpansionAINPC.eAI_SetAccuracy(GetAIMConfig().Missions.Get(m_MissionID).GetAIAccuracy(),GetAIMConfig().Missions.Get(m_MissionID).GetAIAccuracy() + 0.2);
			m_ExpansionAINPC.eAI_SetThreatDistanceLimit(GetAIMConfig().Missions.Get(m_MissionID).GetAIRange());
		}

		//Clothing & Loadout
		
		string ExpansionLoadoutString = GetAIMConfig().Missions.Get(m_MissionID).GetLoadoutName();
		
		bool HasExpansionLoadout = false;
		
		if(ExpansionLoadoutString != "")
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Filling new EAI NPC with Epansion Loadout: " + ExpansionLoadoutString); //Debug
			HasExpansionLoadout = ExpansionHumanLoadout.Apply(m_ExpansionAINPC, ExpansionLoadoutString);
		}
		
		if(!HasExpansionLoadout)
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Dressing up new EAI NPC with random loadout"); //Debug
			
				if(GetAIMConfig().Missions.Get(m_MissionID).IsNBCMission())
			{
				TStringArray nbcLoadout = GetAIMConfig().Loadouts.Get(0).GetNBC_Loadout();
				
				foreach(string NBCitem: nbcLoadout)
				{
					if(NBCitem != "") m_ExpansionAINPC.GetInventory().CreateInInventory(NBCitem);
				}
			}
			else
			{
				m_ExpansionAINPC.GetInventory().CreateInInventory(GetAIMConfig().Loadouts.Get(0).GetShirts().GetRandomElement());
				m_ExpansionAINPC.GetInventory().CreateInInventory(GetAIMConfig().Loadouts.Get(0).GetPants().GetRandomElement());
				m_ExpansionAINPC.GetInventory().CreateInInventory(GetAIMConfig().Loadouts.Get(0).GetShoes().GetRandomElement());
				m_ExpansionAINPC.GetInventory().CreateInInventory(GetAIMConfig().Loadouts.Get(0).GetGloves().GetRandomElement());			
			}
			
			m_ExpansionAINPC.GetInventory().CreateInInventory(GetAIMConfig().Loadouts.Get(0).GetBackPacks().GetRandomElement());
			m_ExpansionAINPC.GetInventory().CreateInInventory(GetAIMConfig().Loadouts.Get(0).GetBelts().GetRandomElement());
			
			int LoadoutID = GetAIMConfig().Missions.Get(m_MissionID).GetLoadoutID();
			
			array<int> allowed_weapons = new array<int>;
			array<int> allowed_armour = new array<int>;
			array<int> allowed_headgear = new array<int>;
			
			for(int i=0; i<GetAIMConfig().Loadouts.Get(0).Weapons.Count(); i++)
			{
				if(GetAIMConfig().Loadouts.Get(0).Weapons.Get(i).GetLoadoutID() == LoadoutID) allowed_weapons.Insert(i);
			}

			for(int j=0; j<GetAIMConfig().Loadouts.Get(0).Armour.Count(); j++)
			{
				if(GetAIMConfig().Loadouts.Get(0).Armour.Get(j).GetLoadoutID() == LoadoutID) allowed_armour.Insert(j);
			}

			for(int k=0; k<GetAIMConfig().Loadouts.Get(0).Headgear.Count(); k++)
			{
				if(GetAIMConfig().Loadouts.Get(0).Headgear.Get(k).GetLoadoutID() == LoadoutID) allowed_headgear.Insert(k);
			}
			
				if(allowed_weapons.Count() > 0)
			{
			int ChosenWeaponID = allowed_weapons.Get(Math.RandomInt(0,allowed_weapons.Count()));

				string Weap_Class = GetAIMConfig().Loadouts.Get(0).Weapons.Get(ChosenWeaponID).GetClassName();
				string Weap_Mag = GetAIMConfig().Loadouts.Get(0).Weapons.Get(ChosenWeaponID).GetMagClassName();
				TStringArray Weap_Attachments = GetAIMConfig().Loadouts.Get(0).Weapons.Get(ChosenWeaponID).GetAttachments();
				
				if(AIM_Debug) Print("[AI Missions] DEBUG - Arming new EAI NPC with weapon '" + Weap_Class + "'."); //Debug

				if(Weap_Class != "")
				{
					tempIB = ItemBase.Cast(m_ExpansionAINPC.GetInventory().CreateInInventory(Weap_Class));
					
					if(tempIB)
					{
						tempEAI = EntityAI.Cast(tempIB);
						if(tempEAI)
						{
							if(Weap_Attachments.Count() > 0)
							{
								foreach(string attach: Weap_Attachments)
								{
									if(attach != "")
									{
										tempEAI.GetInventory().CreateAttachment(attach);
									}
								}
							}
						
							Weapon_Base tempWB = Weapon_Base.Cast(tempIB);
							if(tempWB)
							{
								if(!GetAIMConfig().Missions.Get(m_MissionID).GetAIDropWeapon())
								{
									//Not working
								}
								
								tempWB.SpawnAmmo(Weap_Mag);
								
								m_ExpansionAINPC.GetInventory().CreateInInventory(Weap_Mag);
								m_ExpansionAINPC.GetInventory().CreateInInventory(Weap_Mag);
							}
						}
					}
				}
			}

				if(allowed_armour.Count() > 0)
			{
			int ChosenArmourID = allowed_armour.Get(Math.RandomInt(0,allowed_armour.Count()));
				
				string Armour_Class = GetAIMConfig().Loadouts.Get(0).Armour.Get(ChosenArmourID).GetClassName();
				TStringArray Armour_Attachments = GetAIMConfig().Loadouts.Get(0).Armour.Get(ChosenArmourID).GetAttachments();
				TStringArray Armour_Contents = GetAIMConfig().Loadouts.Get(0).Armour.Get(ChosenArmourID).GetContents();

				if(AIM_Debug) Print("[AI Missions] DEBUG - Putting armour on EAI NPC  '" + Armour_Class + "'."); //Debug

				if(Armour_Class != "")
				{
					tempIB = ItemBase.Cast(m_ExpansionAINPC.GetInventory().CreateInInventory(Armour_Class));
					if(tempIB)
					{
						tempEAI = EntityAI.Cast(tempIB);
						if(tempEAI)
						{
							if(Armour_Attachments.Count() > 0)
							{
								foreach(string arattach: Armour_Attachments)
								{
									if(arattach != "")
									{
										tempEAI.GetInventory().CreateAttachment(arattach);
									}
								}
							}
							
							if(Armour_Contents.Count() > 0)
							{
								foreach(string contents: Armour_Contents)
								{
									if(contents != "")
									{
										tempEAI.GetInventory().CreateInInventory(contents);
									}
								}
							}
						}
					}
				}
			}

				if(allowed_headgear.Count())
			{
			int ChosenHeadgearID = allowed_headgear.Get(Math.RandomInt(0,allowed_headgear.Count()));
				
				string Headgear_Class = GetAIMConfig().Loadouts.Get(0).Headgear.Get(ChosenHeadgearID).GetClassName();
				bool AddNVG = GetAIMConfig().Loadouts.Get(0).Headgear.Get(ChosenHeadgearID).GiveNVG();
				
				if(GetAIMConfig().Missions.Get(m_MissionID).IsNBCMission())
				{
					if(AddNVG)
					{
						if(Class.CastTo(tempEAI,m_ExpansionAINPC.GetInventory().CreateInInventory("NVGHeadstrap")))
						{
							if(Class.CastTo(tempEAI,tempEAI.GetInventory().CreateAttachment("NVGoggles")))
							{
								BatteryCheck(tempEAI);
							}
						}
					}
				}
				else
				{
					string nvgstring = "FALSE";
					
					if(AddNVG) nvgstring = "TRUE";

					if(AIM_Debug) Print("[AI Missions] DEBUG - Putting headgear on EAI NPC  '" + Headgear_Class + "'. Has NVG = " + nvgstring + "."); //Debug

					if(Headgear_Class != "")
					{
						tempIB = ItemBase.Cast(m_ExpansionAINPC.GetInventory().CreateInInventory(Headgear_Class));
						if(tempIB && AddNVG)
						{
							tempEAI = EntityAI.Cast(tempIB);
							if(tempEAI)
							{
								if(Class.CastTo(tempEAI,tempEAI.GetInventory().CreateAttachment("NVGoggles")))
								{
									BatteryCheck(tempEAI);
								}
								else
								{
									if(Class.CastTo(tempEAI,m_ExpansionAINPC.GetInventory().CreateInInventory("NVGHeadstrap")))
									{
										if(Class.CastTo(tempEAI,tempEAI.GetInventory().CreateAttachment("NVGoggles")))
										{
											BatteryCheck(tempEAI);
										}
									}
								}
							}
						}
					}
				}
			}
		}
		
		if(AIM_Debug) Print("[AI Missions] DEBUG - Filling new EAI NPC with random loot and special loot."); //Debug

		//Random & Special Loot
		int RandomLootQty = Math.RandomInt(0, GetAIMConfig().Loadouts.Get(0).GetLootMiscMax());
		for(int l = 0; l < RandomLootQty; l++)
		{
			m_ExpansionAINPC.GetInventory().CreateInInventory(GetAIMConfig().Loadouts.Get(0).GetLootMisc().GetRandomElement());
		}

		int SpecialLootQty = Math.RandomInt(0, GetAIMConfig().Loadouts.Get(0).GetLootSpecialMax());
		for(int m = 0; m < SpecialLootQty; m++)
		{
			m_ExpansionAINPC.GetInventory().CreateInInventory(GetAIMConfig().Loadouts.Get(0).GetLootSpecial().GetRandomElement());
		}
		
		if(AIM_Debug) Print("[AI Missions] DEBUG - SPAWNING NEW EAI NPC SUCCESSFUL"); //Debug

		return m_ExpansionAINPC;
	}

	// Function to clean up mission rewards and objects on server startup and prior to starting a new mission
	void MissionCleanup(int m_MissionID = -1)
	{
		if(AIM_Debug) Print("[AI Missions] DEBUG - Mission cleanup script started!"); //Debug

		if(AIM_IsFirstStartup)
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - First startup cleanup in progress!"); //Debug
			for(int i=0; i<GetAIMConfig().RewardObjects.Count(); i++)
			{
				for(int j=0; j<GetAIMConfig().Missions.Count(); j++)
				{
					array<Object> close_objects = new array<Object>;
					array<CargoBase> close_objects_cargo = new array<CargoBase>;

					GetGame().GetObjectsAtPosition(GetAIMConfig().Missions.Get(j).GetPosition(), GetAIMConfig().Settings.Get(0).GetCleanupRadiusStartup(), close_objects, close_objects_cargo);
						
					for(int k=0; k<close_objects.Count(); k++)
					{
						EntityAI close_object = EntityAI.Cast(close_objects.Get(k));
						if(close_object)
						{
							string tempclassname = GetAIMConfig().RewardObjects.Get(i).GetClassname();
							string tempnewclassname = GetAIMConfig().RewardObjects.Get(i).GetNewClassname();
							
							if(((tempclassname != "") && (close_object.GetType() == tempclassname)) || ((tempnewclassname != "") && (close_object.GetType() == tempnewclassname)))
							{
								bool canDelete = true;
								
								CarScript tempvehicle = CarScript.Cast(close_object);
								if(tempvehicle)
								{
									#ifdef MuchCarKey
									//Check if vehicle owned. Do not delete if it is as someone may have claimed it or parked their vehicle close
									if(tempvehicle.m_CarKeyId != 0) canDelete = false;
									#endif
									
									#ifdef TraderPlus
									#ifdef CarLock
									if(tempvehicle.m_CarLockOwner != -1) canDelete = false;
									#endif
									#endif
								}

								if(canDelete)
								{
									if(AIM_Debug) Print("[AI Missions] DEBUG - Mission cleanup script has located a mission object! Deleting '" + close_object.GetType() + "' at position (" + close_object.GetPosition() + ")."); //Debug
									close_object.Delete();
								}
							}
						}
					}
				}
			}
			
			AIM_IsFirstStartup = false;
		}
		else if((m_MissionID != -1) && !AIM_MissionsArray.Get(m_MissionID).GetIsCleanedUp())
		{
			AIM_MissionsArray.Get(m_MissionID).SetIsCleanedUp();
			
			int m_MissionConfigID = AIM_MissionsArray.Get(m_MissionID).GetMissionID();
				if(AIM_Debug) Print("[AI Missions] DEBUG - Mission cleanup script is now cleaning up reward from ended mission " + m_MissionConfigID); //Debug
			
			EntityAI tempMissionRewardEAI = EntityAI.Cast(AIM_MissionsArray.Get(m_MissionID).GetMissionRewardEAI());
			
				if(tempMissionRewardEAI)
			{
				if(vector.Distance(tempMissionRewardEAI.GetPosition(), GetAIMConfig().Missions.Get(m_MissionConfigID).GetPosition()) <= GetAIMConfig().Settings.Get(0).GetCleanupRadiusRunning())
				{
					int m_MissionRewardID = AIM_MissionsArray.Get(m_MissionID).GetMissionRewardID();
					if(GetAIMConfig().RewardObjects.Get(m_MissionRewardID).GetRewardType() == 2)
					{
						if(AIM_Debug) Print("[AI Missions] DEBUG - Mission cleanup script has located previous mission reward as being a vehicle, and not taken! Setting health of '" + tempMissionRewardEAI.GetType() + "' at position (" + tempMissionRewardEAI.GetPosition() + ") to zero."); //Debug
						
						tempMissionRewardEAI.SetHealth(0.0); //Don't vehicles, but set their health to 0 so they despawn. Stops players getting teleported to the void.
					}
					else
					{
						if(AIM_Debug) Print("[AI Missions] DEBUG - Mission cleanup script has located previous mission reward which was not taken. Deleting '" + tempMissionRewardEAI.GetType() + "' at position (" + tempMissionRewardEAI.GetPosition() + ")."); //Debug

						tempMissionRewardEAI.Delete(); //Delete anything else
					}
				}
			}
			else
			{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Mission cleanup script could not find previous mission reward for cleanup."); //Debug
			}

				if(AIM_MissionsArray.Get(m_MissionID).AIM_ObjectsArray.Count() > 0)
			{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Mission cleanup script is now cleaning up static objects from previous mission."); //Debug
				
				for(int l=0; l<AIM_MissionsArray.Get(m_MissionID).AIM_ObjectsArray.Count(); l++)
				{
					EntityAI TempEAI = EntityAI.Cast(AIM_MissionsArray.Get(m_MissionID).AIM_ObjectsArray.Get(l));
						
					if(TempEAI)
					{
						if(AIM_Debug) Print("[AI Missions] DEBUG - Mission cleanup script has located previous mission static object. Deleting '" + TempEAI.GetType() + "' at position (" + TempEAI.GetPosition() + ")."); //Debug

						TempEAI.Delete();
					}
				}
				
				AIM_MissionsArray.Get(m_MissionID).AIM_ObjectsArray = new array<Object>;
			}
			else
			{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Mission cleanup script could not find previous mission static object(s) for cleanup."); //Debug
			}
		}
		else
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Mission cleanup script did not run (not first startup, or mission ID not provided)!"); //Debug
			return;
		}
				
		if(AIM_Debug) Print("[AI Missions] DEBUG - Mission cleanup script successful!"); //Debug
	}

	// Function to pick a mission reward that's allowed for the chosen mission ID
	int PickMissionReward(int m_MissionID)
	{
		if(AIM_Debug) Print("[AI Missions] DEBUG - Picking mission reward for new mission!"); //Debug

		array<int> allowed_rewards = new array<int>;
		
		for(int i=0; i<GetAIMConfig().RewardObjects.Count(); i++)
		{
				if(GetAIMConfig().RewardObjects.Get(i).GetRewardType() == 0)
			{
				allowed_rewards.Insert(i);
			}
			else if((GetAIMConfig().RewardObjects.Get(i).GetRewardType() == 1) && (GetAIMConfig().Missions.Get(m_MissionID).AllowCrates()))
			{
				allowed_rewards.Insert(i);
			}
			else if((GetAIMConfig().RewardObjects.Get(i).GetRewardType() == 2) && (GetAIMConfig().Missions.Get(m_MissionID).AllowVehicles()))
			{
				allowed_rewards.Insert(i);
			}
			else if((GetAIMConfig().RewardObjects.Get(i).GetRewardType() == 3) && (GetAIMConfig().Missions.Get(m_MissionID).AllowHelicopters()))
			{
				allowed_rewards.Insert(i);
			}
		}
		
		if(allowed_rewards.Count() > 0)
		{
			return allowed_rewards.Get(Math.RandomInt(0, allowed_rewards.Count()));
		}
		
		return -1;
	}

	//Function to spawn the reward at the mission start along with any initial items (I.E parts on vehicles)
	void MissionRewardInit(int m_MissionsArrayID, int m_MissionID, int m_MissionRewardID = -1)
	{
		if(m_MissionRewardID == -1) return; //No reward, only AI to kill
		
		if(AIM_Debug) Print("[AI Missions] DEBUG - Chosen reward ID '" + m_MissionRewardID + "' (" + GetAIMConfig().RewardObjects.Get(m_MissionRewardID).GetClassname() + ") Spawning!"); //Debug

		EntityAI tempreward;

		tempreward = EntityAI.Cast(GetGame().CreateObject(GetAIMConfig().RewardObjects.Get(m_MissionRewardID).GetClassname(), GetAIMConfig().Missions.Get(m_MissionID).GetPosition()));
		if(tempreward)
		{
			AIM_MissionsArray.Get(m_MissionsArrayID).SetMissionRewardEAI(tempreward);

			tempreward.SetOrientation(Vector(Math.RandomInt(0,360),0,0));
			
				if(GetAIMConfig().RewardObjects.Get(m_MissionRewardID).GetRewardType() == 0)
			{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Successfully spawned reward ID '" + m_MissionRewardID + "' (" + GetAIMConfig().RewardObjects.Get(m_MissionRewardID).GetClassname() + ") which is a special (scripted) reward! No loot to fill for now."); //Debug
					return; // Special item, we're done here.
			}
			
				if(GetAIMConfig().RewardObjects.Get(m_MissionRewardID).GetRewardType() == 1)
			{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Successfully spawned reward ID '" + m_MissionRewardID + "' (" + GetAIMConfig().RewardObjects.Get(m_MissionRewardID).GetClassname() + ") which is a crate!"); //Debug
			}

				if(GetAIMConfig().RewardObjects.Get(m_MissionRewardID).GetRewardType() == 2)
			{
				CarScript tempvehicle = CarScript.Cast(tempreward);
				
				if(tempvehicle)
				{
					tempvehicle.LeakAll(CarFluid.FUEL);
					tempvehicle.SetLifetime(GetAIMConfig().Settings.Get(0).GetTimeLimit() + GetAIMConfig().Settings.Get(0).GetTimeExtensionMax());
				}

				if(AIM_Debug) Print("[AI Missions] DEBUG - Successfully spawned reward ID '" + m_MissionRewardID + "' (" + GetAIMConfig().RewardObjects.Get(m_MissionRewardID).GetClassname() + ") which is a vehicle reward! Drained all fuel so it can't easily be taken."); //Debug
			}

				if(GetAIMConfig().RewardObjects.Get(m_MissionRewardID).GetLootInit().Count() > 0)
			{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Attempting to fill reward with " + GetAIMConfig().RewardObjects.Get(m_MissionRewardID).GetLootInit().Count() + " initial items."); //Debug

				for(int i=0; i<GetAIMConfig().RewardObjects.Get(m_MissionRewardID).GetLootInit().Count(); i++)
				{
					if(AIM_Debug) Print("[AI Missions] DEBUG - Adding init item to reward object inventory '" + GetAIMConfig().RewardObjects.Get(m_MissionRewardID).GetLootInit().Get(i) + "'."); //Debug
					tempreward.GetInventory().CreateInInventory(GetAIMConfig().RewardObjects.Get(m_MissionRewardID).GetLootInit().Get(i));
				}
			}

				if(AIM_Debug) Print("[AI Missions] DEBUG - Successfully spawned reward object ID '" + m_MissionRewardID + "'."); //Debug
		}
	}

	//Function to spawn static objects for the mission.
	void MissionObjectsInit(int m_NewMissionID = -1)
	{
		if(m_NewMissionID != -1)
		{
			int m_MissionID = AIM_MissionsArray.Get(m_NewMissionID).GetMissionID();
			int objCount = 0;
			
				if(GetAIMConfig().Missions.Get(m_MissionID).Static_Objects.Count() > 0)
			{
				if(GetAIMConfig().Missions.Get(m_MissionID).Static_Objects.Count() > 0)
				{
					if(AIM_Debug) Print("[AI Missions] DEBUG - Spawning " + GetAIMConfig().Missions.Get(m_MissionID).Static_Objects.Count() + " objects for mission ID '" + m_MissionID + "'"); //Debug

					for(int i=0; i<GetAIMConfig().Missions.Get(m_MissionID).Static_Objects.Count(); i++)
					{
						Object tempObject;
						string classname = GetAIMConfig().Missions.Get(m_MissionID).Static_Objects.Get(i).GetClassName();
						vector position = GetAIMConfig().Missions.Get(m_MissionID).Static_Objects.Get(i).GetPosition();
						vector rotation = GetAIMConfig().Missions.Get(m_MissionID).Static_Objects.Get(i).GetRotation();
						float scale = GetAIMConfig().Missions.Get(m_MissionID).Static_Objects.Get(i).GetScale();
						
						if(AIM_Debug) Print("[AI Missions] DEBUG - Spawning object '" + classname + "' at position (" + position + "), rotation (" + rotation + ") and scale (" + scale + ")."); //Debug

						if(Class.CastTo(tempObject, GetGame().CreateObject(classname, position)))
						{
							tempObject.SetOrientation(rotation);
							tempObject.SetOrientation(tempObject.GetOrientation());
							tempObject.SetScale(scale);
							tempObject.Update();
							tempObject.SetAffectPathgraph(true, false);
							
							if(tempObject.CanAffectPathgraph())
							{
								GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, tempObject);
							}
							
							AIM_MissionsArray.Get(m_NewMissionID).AIM_ObjectsArray.Insert(tempObject);
							
							objCount ++;
						}
					}
				}
				else
				{
					if(AIM_Debug) Print("[AI Missions] DEBUG - Mission ID '" + m_MissionID + "' has no objects."); //Debug
				}
			}
			
				if(AIM_Debug) Print("[AI Missions] DEBUG - Spawned " + objCount + " objects."); //Debug
		}
	}

	// Function to release the mission reward, upon mission success
	void MissionRewardRelease(EntityAI m_MissionRewardEAI, int m_RewardID, int m_MissionID, int m_MissionArrayID)
	{
		if(AIM_Debug) Print("[AI Missions] DEBUG - Releasing mission reward started!"); //Debug
		
		if(!Class.Cast(m_MissionRewardEAI))
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Reward object not found! Unable to release reward.");

			return;
		}
		
		if(m_RewardID == -1)
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Reward not defined! Unable to release reward.");

			return;
		}
		
		if(vector.Distance(m_MissionRewardEAI.GetPosition(), GetAIMConfig().Missions.Get(m_MissionID).GetPosition()) >= GetAIMConfig().Settings.Get(0).GetCleanupRadiusRunning())
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Reward object has been moved outside of cleanup radius! Unable to release reward.");
			
			return;
		}

		if(GetAIMConfig().RewardObjects.Get(m_RewardID).GetNewClassname() != "")
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Swapping mission reward object to '" + GetAIMConfig().RewardObjects.Get(m_RewardID).GetNewClassname() + "'"); //Debug
			
			//Do this first, to replace the reward item if a new classname is defined.
			EntityAI tempnewreward = EntityAI.Cast(GetGame().CreateObject(GetAIMConfig().RewardObjects.Get(m_RewardID).GetNewClassname(), m_MissionRewardEAI.GetPosition()));
			tempnewreward.SetOrientation(m_MissionRewardEAI.GetOrientation());
			
			m_MissionRewardEAI.Delete();
			
			m_MissionRewardEAI = tempnewreward;
			
				if(!Class.Cast(m_MissionRewardEAI))
			{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Swapping mission reward object unsuccessful. Unable to complete releasing reward."); //Debug
					return;
			}
			
				if(AIM_Debug) Print("[AI Missions] DEBUG - Swapping mission reward object successful!"); //Debug
		}

		if(GetAIMConfig().RewardObjects.Get(m_RewardID).GetRewardType() == 0)
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Mission reward object is special (scripted). Reward released successfully."); //Debug
			
			return; // Special item, we're done here.
		}

		if(Class.Cast(m_MissionRewardEAI)) //Check it still exists
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Found mission reward object, filling with loot!"); //Debug
				if(GetAIMConfig().RewardObjects.Get(m_RewardID).GetLootWin().Count() > 0)
			{
				for(int i=0; i<GetAIMConfig().RewardObjects.Get(m_RewardID).GetLootWin().Count(); i++)
				{
					if(AIM_Debug) Print("[AI Missions] DEBUG - Adding item to reward object inventory '" + GetAIMConfig().RewardObjects.Get(m_RewardID).GetLootWin().Get(i) + "'"); //Debug
					
					m_MissionRewardEAI.GetInventory().CreateInInventory(GetAIMConfig().RewardObjects.Get(m_RewardID).GetLootWin().Get(i));
				}
			}

		FillRewards(m_MissionRewardEAI, 0, AIM_MissionsArray.Get(m_MissionArrayID).GetTotalBots(), m_MissionID);
		FillRewards(m_MissionRewardEAI, 1, AIM_MissionsArray.Get(m_MissionArrayID).GetTotalBots(), m_MissionID);
		FillRewards(m_MissionRewardEAI, 2, AIM_MissionsArray.Get(m_MissionArrayID).GetTotalBots(), m_MissionID);

				if(GetAIMConfig().RewardObjects.Get(m_RewardID).GetRewardType() == 2)
			{
				CarScript tempvehicle = CarScript.Cast(m_MissionRewardEAI);
				if(tempvehicle)
				{
					float fuelReq = tempvehicle.GetFluidCapacity(CarFluid.FUEL) - (tempvehicle.GetFluidCapacity(CarFluid.FUEL) * tempvehicle.GetFluidFraction(CarFluid.FUEL));
					float oilReq = tempvehicle.GetFluidCapacity(CarFluid.OIL) - (tempvehicle.GetFluidCapacity(CarFluid.OIL) * tempvehicle.GetFluidFraction(CarFluid.OIL));
					float coolantReq = tempvehicle.GetFluidCapacity(CarFluid.COOLANT) - (tempvehicle.GetFluidCapacity(CarFluid.COOLANT) * tempvehicle.GetFluidFraction(CarFluid.COOLANT));
					float brakeReq = tempvehicle.GetFluidCapacity(CarFluid.BRAKE) - (tempvehicle.GetFluidCapacity(CarFluid.BRAKE) * tempvehicle.GetFluidFraction(CarFluid.BRAKE));
					tempvehicle.Fill(CarFluid.FUEL, fuelReq);
					tempvehicle.Fill(CarFluid.OIL, oilReq);
					tempvehicle.Fill(CarFluid.COOLANT, coolantReq);
					tempvehicle.Fill(CarFluid.BRAKE, brakeReq);
				}
			}
			}
	}

		//Function to fill reward with defined items. Condensed to one function
		void FillRewards(EntityAI m_MissionRewardEAI, int m_RewardType, int m_AIQuantity, int m_MissionID)
		{
			int rewardqty = 0;
				if (GetAIMConfig().Loot.Count() == 0)
				{
					return;
				}
		if(m_RewardType == 0) {	//weapons
			rewardqty = Math.Round(m_AIQuantity * GetAIMConfig().Settings.Get(0).GetLootWeaponsMultiplier());
				if(rewardqty < GetAIMConfig().Settings.Get(0).GetLootWeaponsMin()) rewardqty = GetAIMConfig().Settings.Get(0).GetLootWeaponsMin();
				if(rewardqty > GetAIMConfig().Settings.Get(0).GetLootWeaponsMax()) rewardqty = GetAIMConfig().Settings.Get(0).GetLootWeaponsMax();

				if(AIM_Debug) Print("[AI Missions] DEBUG - Spawning " + rewardqty + " weapons into reward object inventory."); //Debug
		}
		else if(m_RewardType == 1) //armour
		{
			rewardqty = Math.Round(m_AIQuantity * GetAIMConfig().Settings.Get(0).GetLootArmourMultiplier());
				if(rewardqty < GetAIMConfig().Settings.Get(0).GetLootArmourMin()) rewardqty = GetAIMConfig().Settings.Get(0).GetLootArmourMin();
				if(rewardqty > GetAIMConfig().Settings.Get(0).GetLootArmourMax()) rewardqty = GetAIMConfig().Settings.Get(0).GetLootArmourMax();

				if(AIM_Debug) Print("[AI Missions] DEBUG - Spawning " + rewardqty + " armour into reward object inventory."); //Debug
		}
		else if(m_RewardType == 2) //misc
		{
			rewardqty = Math.Round(m_AIQuantity * GetAIMConfig().Settings.Get(0).GetLootMiscMultiplier());
				if(rewardqty < GetAIMConfig().Settings.Get(0).GetLootMiscMin()) rewardqty = GetAIMConfig().Settings.Get(0).GetLootMiscMin();
				if(rewardqty > GetAIMConfig().Settings.Get(0).GetLootMiscMax()) rewardqty = GetAIMConfig().Settings.Get(0).GetLootMiscMax();

				if(AIM_Debug) Print("[AI Missions] DEBUG - Spawning " + rewardqty + " misc into reward object inventory."); //Debug
}

			int missionLootTier = GetAIMConfig().Missions.Get(m_MissionID).GetLootTier();
			missionLootTier = Math.Clamp(missionLootTier, 0, GetAIMConfig().Loot.Count() - 1);

			for(int i=0; i<rewardqty; i++)
			{
				int randnum;
				string classname;
				string magclassname;
			TStringArray attachments = new TStringArray();
			TStringArray loot = new TStringArray();
			EntityAI tempreward;
			EntityAI tempEAI;
			
if(m_RewardType == 0) //weapons
			{randnum = Math.RandomInt(0,GetAIMConfig().Loot.Get(missionLootTier).Weapons.Count());
classname = GetAIMConfig().Loot.Get(missionLootTier).Weapons.Get(randnum).GetClassname();
magclassname = GetAIMConfig().Loot.Get(missionLootTier).Weapons.Get(randnum).GetMagClassName();
attachments = GetAIMConfig().Loot.Get(missionLootTier).Weapons.Get(randnum).GetAttachments();
loot = GetAIMConfig().Loot.Get(missionLootTier).Weapons.Get(randnum).GetLoot();
}
else if(m_RewardType == 1) //armour
			{randnum = Math.RandomInt(0,GetAIMConfig().Loot.Get(missionLootTier).Armour.Count());
classname = GetAIMConfig().Loot.Get(missionLootTier).Armour.Get(randnum).GetClassname();
attachments = GetAIMConfig().Loot.Get(missionLootTier).Armour.Get(randnum).GetAttachments();
loot = GetAIMConfig().Loot.Get(missionLootTier).Armour.Get(randnum).GetLoot();
}
else if(m_RewardType == 2) //misc
			{randnum = Math.RandomInt(0,GetAIMConfig().Loot.Get(missionLootTier).Misc.Count());
classname = GetAIMConfig().Loot.Get(missionLootTier).Misc.Get(randnum).GetClassname();
attachments = GetAIMConfig().Loot.Get(missionLootTier).Misc.Get(randnum).GetAttachments();
loot = GetAIMConfig().Loot.Get(missionLootTier).Misc.Get(randnum).GetLoot();
}

				if(AIM_Debug) Print("[AI Missions] DEBUG - Adding item to reward object inventory '" + classname + "'"); //Debug

				if(Class.CastTo(tempreward, m_MissionRewardEAI.GetInventory().CreateInInventory(classname)))	// NEW <- At this line you have a freaking awesome trouble it's missed ')', that ruins of code!
			{
				Weapon_Base weapontemp;
				if(Class.CastTo(weapontemp, tempreward))
				{
					if(AIM_Debug) Print("[AI Missions] DEBUG - Reward item detected as being a weapon, attaching random magazine and filling with ammo."); //Debug
					
					if(magclassname != "")
					{
						weapontemp.SpawnAmmo(magclassname);
					}
					else
					{
						weapontemp.SpawnAmmo();
					}
				}

				if(attachments.Count() > 0)
				{
					for(int j=0; j<attachments.Count(); j++)
					{
						if(AIM_Debug) Print("[AI Missions] DEBUG - Attaching '" + attachments.Get(j) + "' to item."); //Debug

						if(Class.CastTo(tempEAI, tempreward.GetInventory().CreateInInventory(attachments.Get(j))))
						{
							BatteryCheck(tempEAI);
						}
					}
				}

				if(loot.Count() > 0)
				{
					for(int k=0; k<loot.Count(); k++)
					{

						if(m_RewardType == 0)
						{
							if(Class.CastTo(tempEAI, m_MissionRewardEAI.GetInventory().CreateInInventory(loot.Get(k))))
							{
								//Weapon, so fill the original reward item
								
								if(AIM_Debug) Print("[AI Missions] DEBUG - Spawning '" + loot.Get(k) + "' in reward item's inventory."); //Debug
								
								BatteryCheck(tempEAI);
							}
						}
						else
						{
							if(Class.CastTo(tempEAI, tempreward.GetInventory().CreateInInventory(loot.Get(k))))
							{
								//Not a weapon, so fill the spawned item itself
								
								if(AIM_Debug) Print("[AI Missions] DEBUG - Spawning '" + loot.Get(k) + "' in item's inventory."); //Debug
								
								BatteryCheck(tempEAI);
							}
						}
					}
				}
			}
			else
			{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Failed to spawn reward item into reward object '" + classname + "'."); //Debug
			}
		}
		
		if(m_RewardType == 0) //weapons
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Successfully spawned all weapons into reward object."); //Debug
		}
		else if(m_RewardType == 1) //armour
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Successfully spawned all armour into reward object."); //Debug
		}
		else if(m_RewardType == 2) //misc
		{
				if(AIM_Debug) Print("[AI Missions] DEBUG - Successfully spawned all misc into reward object."); //Debug
		}
	}
	
	// Function to automatically attach a battery, if it's a powered scope or NVG
	void BatteryCheck(EntityAI tempEAI)
	{
		if(!tempEAI) return;
		
		if(tempEAI.HasEnergyManager())
		{
			tempEAI.GetInventory().CreateAttachment("Battery9V");
		}
	}
}

class AIM_Missions
			{	ref array<eAIBase> AIM_EAIArray = new array<eAIBase>;
	ref array<int> AIM_EAIWanderTimeArray = new array<int>;
	ref array<Object> AIM_ObjectsArray = new array<Object>;
	
	int AIM_MissionID = -1;
	int AIM_MissionRewardID = -1;
	int AIM_LBMarker = -1;
	int AIM_MissionTime = 0;
	int AIM_MissionTimeExtension = 0;
	int AIM_MissionRemainingBots = 0;
	int AIM_MissionTotalBots = 0;

	bool AIM_MissionPendingFinish = false;
	bool AIM_MissionIsEnded = false;

	EntityAI AIM_MissionRewardEAI;

	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerData AIM_ENavMarker;
	#endif

	void AIM_Missions(int m_MissionID, int m_MissionRewardID, int m_MissionBots)
	{
		AIM_EAIArray = new array<eAIBase>;
		AIM_EAIWanderTimeArray = new array<int>;
		AIM_ObjectsArray = new array<Object>;
		
		AIM_MissionID = m_MissionID;
		AIM_MissionRewardID = m_MissionRewardID;
		AIM_MissionTime = 0;
		AIM_MissionTimeExtension = 0;
		AIM_MissionRemainingBots = m_MissionBots;
		AIM_MissionTotalBots = m_MissionBots;
		
		AIM_MissionPendingFinish = false;
		AIM_MissionIsEnded = false;
	}

	void ~AIM_Missions()
	{
		if(this.AIM_EAIArray) this.AIM_EAIArray.Clear();
		this.AIM_EAIArray = NULL;
		if(this.AIM_EAIWanderTimeArray) this.AIM_EAIWanderTimeArray.Clear();
		this.AIM_EAIWanderTimeArray = NULL;
		if(this.AIM_ObjectsArray) this.AIM_ObjectsArray.Clear();
		this.AIM_ObjectsArray = NULL;
		this.AIM_MissionID = -1;
		this.AIM_MissionRewardID = 0;
		this.AIM_LBMarker = 0;
		this.AIM_MissionTime = 0;
		this.AIM_MissionTimeExtension = 0;
		this.AIM_MissionRemainingBots = 0;
		this.AIM_MissionTotalBots = 0;
		this.AIM_MissionPendingFinish = false;
		this.AIM_MissionIsEnded = false;
		this.AIM_MissionRewardEAI = NULL;
		#ifdef EXPANSIONMODNAVIGATION
		this.AIM_ENavMarker = NULL;
		#endif
	}
	
	int GetMissionID()
	{
		return AIM_MissionID;
	}
	
	int GetMissionRewardID()
	{
		return AIM_MissionRewardID;
	}
	
	void SetLBMarkerID(int markerID)
	{
		AIM_LBMarker = markerID;
	}
	
	int GetLBMarkerID()
	{
		return AIM_LBMarker;
	}
	
	int GetCurrentMissionTime()
	{
		return AIM_MissionTime;
	}
	
	void SetCurrentMissionTime(int timeSet)
	{
		AIM_MissionTime = timeSet;
	}
	
	int GetCurrentMissionTimeExtension()
	{
		return AIM_MissionTimeExtension;
	}
	
	void SetCurrentMissionTimeExtension(int timeSet)
	{
		AIM_MissionTimeExtension = timeSet;
	}
	
	int GetRemainingBots()
	{
		return AIM_MissionRemainingBots;
	}
	
	void SetRemainingBots(int setCount)
	{
		AIM_MissionRemainingBots = setCount;
	}

	int GetTotalBots()
	{
		return AIM_MissionTotalBots;
	}

	void SetMissionPendingFinish(bool setState)
	{
		AIM_MissionPendingFinish = setState;
	}

	bool IsMissionPendingFinish()
	{
		return AIM_MissionPendingFinish;
	}
	
	bool GetIsCleanedUp()
	{
		return AIM_MissionIsEnded;
	}
	
	void SetIsCleanedUp()
	{
		AIM_MissionIsEnded = true;
	}
	
	EntityAI GetMissionRewardEAI()
	{
		return AIM_MissionRewardEAI;
	}
	
	void SetMissionRewardEAI(EntityAI setEAI)
	{
		if(setEAI != NULL) AIM_MissionRewardEAI = setEAI;
	}

	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerData GetExpansionMarkerData()
	{
		return AIM_ENavMarker;
	}
	
	void SetExpansionMarkerData(ExpansionMarkerData setData)
	{
		AIM_ENavMarker = setData;
	}
	#endif
}

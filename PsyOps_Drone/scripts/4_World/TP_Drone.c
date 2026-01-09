class TP_Drone extends ItemBase
{
    ref Timer myTimer;
    vector currentPosition;
    vector initialPosition;
    PlayerBase targetPlayer;
    float followDistance = 2.5;
    float hoverHeight = 2.0;
    float targetHeight = 200.0;
    float movementSpeed = 0.1;
    float ascentSpeed = 0.1;
    bool crateAttached = false;
    Container_Base DronewoodenCrate;
    ref array<ref DeliveryKitItem> mt_LootArray; 
    protected EffectSound m_DroneEventSound;

    void TP_Drone()
    {
        SetEventMask(EntityEvent.INIT); 
    }
    
    void ~TP_Drone()
    {
        if (m_DroneEventSound)
        {
            m_DroneEventSound.SoundStop();
        }
    }

    override void EEInit()
    {
        super.EEInit();

        if (!GetGame().IsDedicatedServer())
        {
            TP_PlaySound();
        }
    }

    void TP_PlaySound()
    {
        bool soundPlayed = PlaySoundSetLoop(m_DroneEventSound, "drone_soundset", 0.1, 0.1);
    }

    void InitDrone(PlayerBase player, ref array<ref DeliveryKitItem> lootItems) 
    {
        targetPlayer = player;
        mt_LootArray = lootItems;
        myTimer = new Timer(CALL_CATEGORY_SYSTEM);
        initialPosition = this.GetPosition();
        StartFollowTimer(0.01);
    }

	void AttachCrate()
	{
		Container_Base crate = Container_Base.Cast(GetGame().CreateObject("DeliveryCrate_Yellow", "0 0 0", false, false));

		if (crate)
		{
			vector positionOffset = "0 -0.17 0";
			vector orientation = "90 0 0";
			AttachTo(crate, this, positionOffset, orientation, "spine1");
			DronewoodenCrate = crate;
			DronewoodenCrate.SetHealth("", "", 0);
		}
	}

	
    void SendNotificationToPlayer(PlayerBase player, string title, string message, string icon) {
        if (player && message != "") {
            NotificationSystem.SendNotificationToPlayerExtended(player, 5, title, message, icon);
        }
    }

    void StartAscendTimer()
    {
        myTimer.Run(0.01, this, "AscendToHeight", NULL, true);
    }

    void AscendToHeight() {
        currentPosition = this.GetPosition();

        float mt_surfaceHeight = GetGame().SurfaceY(currentPosition[0], currentPosition[2]);
        float mt_targetHeight = mt_surfaceHeight + 200; 

        if (currentPosition[1] < mt_targetHeight) {
            currentPosition[1] = currentPosition[1] + ascentSpeed;
            this.SetPosition(currentPosition);
        } else {
            StopAscendTimer();
            DeleteSafe();
        }
    }

    void StopAscendTimer()
    {
        if (myTimer.IsRunning())
        {
            myTimer.Stop();
        }
    }

    void FollowPlayer() {
        if (!targetPlayer) return;

        SetSynchDirty();
        vector playerPos = targetPlayer.GetPosition();
        currentPosition = this.GetPosition();

        float distanceToPlayer = vector.Distance(currentPosition, playerPos);

        if (distanceToPlayer <= 99.0 && !crateAttached) {
            AttachCrate();
            SendNotificationToPlayer(targetPlayer, "Drone Delivery", 
                "Survivor, your drone is close. Look to the sky.", "set:dayz_gui image:icon_error"); 
            crateAttached = true;
        }

        float groundHeightAtPlayer = playerPos[1];
		//GetGame().SurfaceY(playerPos[0], playerPos[2]);
        
		float desiredHeight = groundHeightAtPlayer + hoverHeight;
        vector targetPosition = Vector(playerPos[0], desiredHeight, playerPos[2]);
        vector direction = vector.Direction(currentPosition, targetPosition).Normalized();
        vector moveStep = direction * movementSpeed;
        currentPosition = currentPosition + moveStep;
        this.SetPosition(currentPosition);

        if (vector.Distance(currentPosition, targetPosition) <= followDistance) {
            StopFollowTimer();
            DropCrate();
        }
    }

    void DropCrate()
    {
        if (DronewoodenCrate)
        {
            this.RemoveChild(DronewoodenCrate);
			vector ph = targetPlayer.GetPosition();
            vector dropPosition = this.GetPosition();
            dropPosition[1] = ph[1];
			//GetGame().SurfaceY(dropPosition[0], dropPosition[2]);
            DronewoodenCrate.SetPosition(dropPosition);
            FillCrateWithLoot(DronewoodenCrate, mt_LootArray); 
            SetSynchDirty();
            StartAscendTimer();
        }
    }

    void FillCrateWithLoot(Container_Base crate, ref array<ref DeliveryKitItem> lootItems) 
    {
        if (crate)
        {
            foreach (DeliveryKitItem kitItem : lootItems)
            {
                if (!kitItem) {
                    continue;
                }

                string itemName = kitItem.class_name;
                int quantity = kitItem.quantity;
                if (quantity < 1) {
                    quantity = 1;
                }

                for (int i = 0; i < quantity; i++) {
                    crate.GetInventory().CreateInInventory(itemName);
                }
            }
        }
    }

    void StartFollowTimer(float interval)
    {
        myTimer.Run(interval, this, "FollowPlayer", NULL, true);
    }

    void StopFollowTimer()
    {
        if (myTimer.IsRunning())
        {
            myTimer.Stop();
        }
    }

    static void AttachTo(notnull EntityAI child, notnull EntityAI parent, vector local_pos = "0 0 0", vector orientation = "0 0 0", string mem_point = "")
    {
        if (mem_point != "" && parent.MemoryPointExists(mem_point))
        {
            local_pos += parent.GetMemoryPointPos(mem_point);
        }

        child.SetOrientation(orientation);
        child.SetPosition(local_pos);
        parent.AddChild(child, -1);
        parent.Update();
    }
}

class DroneSpawner
{
    void SpawnDrone(PlayerBase targetPlayer, vector spawnPosition, ref array<ref DeliveryKitItem> lootItems)
    {
        TP_Drone drone = TP_Drone.Cast(GetGame().CreateObject("TP_Drone", spawnPosition));

        if (drone)
        {
            drone.InitDrone(targetPlayer, lootItems);
        }
    }
}

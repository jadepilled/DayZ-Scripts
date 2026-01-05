class CyberwareCirculatoryBase: Clothing
{
    override void OnWasAttached(EntityAI parent, int slot_id)
    {
        super.OnWasAttached(parent, slot_id);
        PlayerBase player = PlayerBase.Cast(parent);
        if (player)
        {
            OnAttachedToPlayer(player);
        }
    }

    override void OnWasDetached(EntityAI parent, int slot_id)
    {
        super.OnWasDetached(parent, slot_id);
        PlayerBase player = PlayerBase.Cast(parent);
        if (player)
        {
            OnDetachedFromPlayer(player);
        }
    }

    protected void OnAttachedToPlayer(PlayerBase player) {}
    protected void OnDetachedFromPlayer(PlayerBase player) {}
}

class BloodPump: CyberwareCirculatoryBase
{
    protected const float TICK_RATE_MS = 20000; // 20 seconds

    override void OnAttachedToPlayer(PlayerBase player)
    {
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.HealTick, TICK_RATE_MS, true, player);
    }

    override void OnDetachedFromPlayer(PlayerBase player)
    {
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.HealTick);
    }

    void HealTick(PlayerBase player)
    {
        if (!player)
        {
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.HealTick);
            return;
        }

        player.AddHealth("","",1);
    }
}

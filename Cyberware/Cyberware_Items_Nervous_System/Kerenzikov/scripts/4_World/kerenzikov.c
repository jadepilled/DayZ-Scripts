class CyberwareNervousBase: Clothing
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

class Kerenzikov: CyberwareNervousBase
{
    protected const float STAMINA_DEPLETION_MULTIPLIER = 0.85; // 15% reduction
    protected float m_PreviousDepletionMultiplier = 1.0;

    override void OnAttachedToPlayer(PlayerBase player)
    {
        StaminaHandler handler = player.GetStaminaHandler();
        if (handler)
        {
            m_PreviousDepletionMultiplier = handler.GetDepletionMultiplier();
            handler.SetDepletionMultiplier(m_PreviousDepletionMultiplier * STAMINA_DEPLETION_MULTIPLIER);
        }
    }

    override void OnDetachedFromPlayer(PlayerBase player)
    {
        StaminaHandler handler = player.GetStaminaHandler();
        if (handler)
        {
            handler.SetDepletionMultiplier(m_PreviousDepletionMultiplier);
        }
    }
}

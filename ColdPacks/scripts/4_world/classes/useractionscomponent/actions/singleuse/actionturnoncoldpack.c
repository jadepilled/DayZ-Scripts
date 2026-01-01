class ActionTurnOnColdpack : ActionTurnOnWhileInHands
{
        void ActionTurnOnColdpack()
        {
                m_Text = "#activate";
        }

        override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
        {
                bool canActivate = super.ActionCondition(player, target, item);

                if (canActivate && item && item.IsInherited(ColdPack))
                {
                        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_HEATPACK;
                        m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_HEATPACK;
                }

                return canActivate;
        }
}

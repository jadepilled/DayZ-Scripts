class ActionTurnOnColdPack : ActionTurnOnWhileInHands
{
        void ActionTurnOnColdPack()
        {
                m_Text = "#activate";
                m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_HEATPACK;
                m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_HEATPACK;
        }
}

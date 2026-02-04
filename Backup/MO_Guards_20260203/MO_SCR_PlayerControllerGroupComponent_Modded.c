modded class SCR_PlayerControllerGroupComponent
{
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	override void RPC_DoChangeGroupID(int groupID)
	{
		m_iGroupID = groupID;
		if (groupID == m_iGroupInviteID)
		{
			// reset the invite if player manually joined the group he is invited into
			m_iGroupInviteID = -1;
			if (m_OnInviteCancelled)
				m_OnInviteCancelled.Invoke();
		}
		
		if (!m_OnGroupChanged)
			return;
		
		MO_GameModeStateSystem moState = MO_GameModeStateSystem.GetInstance();
		if (moState && moState.callQueue)
		{
			moState.callQueue.Call(OnGroupChangedDelayed, groupID);
			return;
		}
		
		// Fallback if MO system not initialized yet.
		GetGame().GetCallqueue().Call(OnGroupChangedDelayed, groupID);
	}
}

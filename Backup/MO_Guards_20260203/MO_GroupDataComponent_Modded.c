modded class MO_GroupDataComponent
{
	protected static const int SCORE_RETRY_MS = 250;
	protected static const int SCORE_RETRY_MAX = 40;
	protected int m_iScoreRetryCount = 0;

	override void EOnInit(IEntity owner)
	{
		group = SCR_AIGroup.Cast(owner);
		if (!group)
		{
			PrintFormat("MO: MO_GroupDataComponent.EOnInit(%1): Not an SCR_AIGroup!", LogLevel.WARNING);
			return;
		}

		// only run init on server, changes should be replicated to clients
		if (!Replication.IsServer())
			return;

		PersistenceSystem persistence = PersistenceSystem.GetInstance();
		if (!persistence)
			return;

		// set uuid so it gets replicated for client-side "is friendly?" checks against GACs of other entities
		groupUuid = persistence.GetId(owner);

		TryUpdateScore();
	}

	protected void TryUpdateScore()
	{
		MO_ScoringSystem scoring = MO_ScoringSystem.GetInstance();
		if (scoring)
		{
			scoring.UpdateScore(groupUuid, groupScore);
			return;
		}

		if (m_iScoreRetryCount >= SCORE_RETRY_MAX)
			return;

		m_iScoreRetryCount++;

		if (GetGame())
			GetGame().GetCallqueue().CallLater(TryUpdateScore, SCORE_RETRY_MS, false);
	}
}

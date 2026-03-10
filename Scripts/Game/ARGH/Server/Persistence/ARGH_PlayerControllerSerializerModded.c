modded class SCR_PlayerControllerSerializer : ScriptedEntitySerializer
{
	override protected ESerializeResult Serialize(notnull IEntity entity, notnull BaseSerializationSaveContext context)
	{
		const SCR_PlayerController playerController = SCR_PlayerController.Cast(entity);
		if (!playerController)
			return ESerializeResult.ERROR;

		UUID playerEntity = GetSystem().GetId(playerController.GetMainEntity());
		UUID currentEntity = GetSystem().GetId(playerController.GetControlledEntity());
		if (currentEntity.IsNull())
			currentEntity = playerEntity;

		// In MercOut reconnect flows, MainEntity can be null while ControlledEntity is valid.
		// Persist the controlled entity as playerEntity so reconnect restore has a stable character id.
		if (playerEntity.IsNull() && !currentEntity.IsNull())
			playerEntity = currentEntity;

		if (playerEntity.IsNull() && currentEntity.IsNull() && Replication.IsServer() && ARGH_HiddenRandomSpawnService.IsMercOutScenario())
		{
			UUID identityId = SCR_PlayerIdentityUtils.GetPlayerIdentityId(playerController.GetPlayerId());
			if (!identityId.IsNull())
			{
				ARGH_PlayerCharLinkComponent registry = ARGH_PlayerCharLinkComponent.GetInstance();
				if (registry)
				{
					string characterIdString;
					string identityString = string.Format("%1", identityId);
					if (registry.TryGetLink(identityString, characterIdString))
					{
						UUID repairedCharacterId = characterIdString;
						if (!repairedCharacterId.IsNull())
						{
							playerEntity = repairedCharacterId;
							currentEntity = repairedCharacterId;
							Print(string.Format("[ARGH_PERSIST] player=%1 action=controller-link-recovered identity='%2' character='%3'", playerController.GetPlayerId(), identityString, characterIdString), LogLevel.WARNING);
						}
					}
				}
			}
		}

		if (playerEntity.IsNull() && (!m_bPosessionPossible || currentEntity == playerEntity))
			return ESerializeResult.DEFAULT;

		context.WriteValue("version", 1);
		context.WriteDefault(playerEntity, UUID.NULL_UUID);

		if (m_bPosessionPossible)
			context.WriteDefault(currentEntity, playerEntity);

		return ESerializeResult.OK;
	}
}

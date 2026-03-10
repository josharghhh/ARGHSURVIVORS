[BaseContainerProps(category: "Respawn")]
modded class SCR_SpawnLogic
{
	protected static const int ARGH_RESTORE_TIMEOUT_MS = 5000;
	protected static const int ARGH_AUDIT_SPAWN_STALL_TIMEOUT_MS = 12000;
	protected static const int ARGH_DISCONNECT_FLUSH_DELAY_MS = 250;
	protected static const int ARGH_DISCONNECT_FLUSH_THROTTLE_MS = 1000;
	protected static const FactionKey ARGH_JOIN_FORCED_FACTION_KEY = "PLAYERS";
	protected static const int ARGH_JOIN_FORCE_FACTION_RETRY_MS = 250;
	protected static const int ARGH_JOIN_FORCE_FACTION_MAX_RETRIES = 12;

	protected ref map<int, int> m_mARGH_RestoreTimeoutTokens = new map<int, int>();
	protected ref map<int, int> m_mARGH_AuditSpawnStallTokens = new map<int, int>();
	protected ref map<int, int> m_mARGH_JoinFactionTokens = new map<int, int>();
	protected ref set<int> m_aARGH_RestoreSpawnPending = new set<int>();
	protected int m_iARGH_NextRestoreToken = 1;
	protected int m_iARGH_NextAuditStallToken = 1;
	protected int m_iARGH_NextJoinFactionToken = 1;

	//------------------------------------------------------------------------------------------------
	protected bool ARGH_UsesMercOutRestoreFlow()
	{
		return ARGH_HiddenRandomSpawnService.IsMercOutScenario();
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_Log(string message)
	{
		Print(string.Format("[ARGH_PERSIST] %1", message), LogLevel.WARNING);
	}

	//------------------------------------------------------------------------------------------------
	protected SCR_PlayerController ARGH_GetPlayerController_S(int playerId)
	{
		return SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_SetRespawnMode_S(int playerId, ARGH_RespawnMode mode)
	{
		SCR_PlayerController playerController = ARGH_GetPlayerController_S(playerId);
		if (playerController)
			playerController.ARGH_SetRespawnMode_S(mode);
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_SetRestorePending_S(int playerId, bool pending)
	{
		SCR_PlayerController playerController = ARGH_GetPlayerController_S(playerId);
		if (playerController)
			playerController.ARGH_SetRestorePending_S(pending);
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_RequestForceJoinFaction_S(int playerId, string reason)
	{
		if (!ARGH_UsesMercOutRestoreFlow())
			return;

		int token = m_iARGH_NextJoinFactionToken++;
		m_mARGH_JoinFactionTokens.Set(playerId, token);
		ARGH_ForceJoinFactionAttempt_S(playerId, reason, token, 0);
	}

	//------------------------------------------------------------------------------------------------
	protected bool ARGH_HasJoinFactionRequest_S(int playerId, int token = -1)
	{
		int activeToken;
		if (!m_mARGH_JoinFactionTokens.Find(playerId, activeToken))
			return false;

		if (token != -1 && activeToken != token)
			return false;

		return true;
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_ClearJoinFactionRequest_S(int playerId)
	{
		m_mARGH_JoinFactionTokens.Remove(playerId);
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_ForceJoinFactionAttempt_S(int playerId, string reason, int token, int attempt)
	{
		if (!ARGH_HasJoinFactionRequest_S(playerId, token))
			return;

		SCR_PlayerController playerController = ARGH_GetPlayerController_S(playerId);
		if (!playerController)
		{
			ARGH_ClearJoinFactionRequest_S(playerId);
			return;
		}

		SCR_PlayerFactionAffiliationComponent factionComponent = SCR_PlayerFactionAffiliationComponent.Cast(playerController.FindComponent(SCR_PlayerFactionAffiliationComponent));
		SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionComponent || !factionManager)
		{
			if (attempt >= ARGH_JOIN_FORCE_FACTION_MAX_RETRIES)
			{
				ARGH_Log(string.Format("player=%1 action=join-faction-force-timeout reason='%2' detail='components-pending' attempts=%3", playerId, reason, attempt));
				ARGH_ClearJoinFactionRequest_S(playerId);
				return;
			}

			if (attempt == 0)
				ARGH_Log(string.Format("player=%1 action=join-faction-force-retry reason='%2' detail='components-pending'", playerId, reason));

			GetGame().GetCallqueue().CallLater(ARGH_ForceJoinFactionAttempt_S, ARGH_JOIN_FORCE_FACTION_RETRY_MS, false, playerId, reason, token, attempt + 1);
			return;
		}

		FactionKey resolvedFactionKey = ARGH_JOIN_FORCED_FACTION_KEY;
		SCR_FactionAliasComponent factionAliasComponent = SCR_FactionAliasComponent.Cast(factionManager.FindComponent(SCR_FactionAliasComponent));
		if (factionAliasComponent)
			resolvedFactionKey = factionAliasComponent.ResolveFactionAlias(resolvedFactionKey);

		Faction forcedFaction = factionManager.GetFactionByKey(resolvedFactionKey);
		if (!forcedFaction)
		{
			if (attempt >= ARGH_JOIN_FORCE_FACTION_MAX_RETRIES)
			{
				ARGH_Log(string.Format("player=%1 action=join-faction-force-timeout reason='%2' detail='faction-missing' configured='%3' resolved='%4' attempts=%5", playerId, reason, ARGH_JOIN_FORCED_FACTION_KEY, resolvedFactionKey, attempt));
				ARGH_ClearJoinFactionRequest_S(playerId);
				return;
			}

			if (attempt == 0)
				ARGH_Log(string.Format("player=%1 action=join-faction-force-retry reason='%2' detail='faction-missing' configured='%3' resolved='%4'", playerId, reason, ARGH_JOIN_FORCED_FACTION_KEY, resolvedFactionKey));

			GetGame().GetCallqueue().CallLater(ARGH_ForceJoinFactionAttempt_S, ARGH_JOIN_FORCE_FACTION_RETRY_MS, false, playerId, reason, token, attempt + 1);
			return;
		}

		factionComponent.RequestFaction(forcedFaction);
		ARGH_Log(string.Format("player=%1 action=join-faction-forced configured='%2' resolved='%3' reason='%4' attempt=%5", playerId, ARGH_JOIN_FORCED_FACTION_KEY, forcedFaction.GetFactionKey(), reason, attempt));
		ARGH_ClearJoinFactionRequest_S(playerId);
	}

	//------------------------------------------------------------------------------------------------
	protected bool ARGH_TryGetIdentityString_S(int playerId, out string identityString)
	{
		identityString = string.Empty;

		UUID identityId = SCR_PlayerIdentityUtils.GetPlayerIdentityId(playerId);
		if (identityId.IsNull())
			return false;

		identityString = string.Format("%1", identityId);
		return !identityString.IsEmpty();
	}

	//------------------------------------------------------------------------------------------------
	protected bool ARGH_HasPersistedPlayerState_S(int playerId, SCR_PlayerController playerController, UUID playerCharacterId = UUID.NULL_UUID, UUID controlledCharacterId = UUID.NULL_UUID)
	{
		if (!ARGH_UsesMercOutRestoreFlow() || !playerController)
			return false;

		// A player already controlling a live entity in this session should never be reclassified as a reconnect restore.
		if (ARGH_HiddenRandomSpawnService.HasAliveControlledEntity(playerController))
			return false;

		if (!playerCharacterId.IsNull() || !controlledCharacterId.IsNull())
			return true;

			ARGH_PlayerCharLinkComponent registry = ARGH_PlayerCharLinkComponent.GetInstance();
		if (!registry)
			return false;

		string identityString;
		if (!ARGH_TryGetIdentityString_S(playerId, identityString))
			return false;

		string characterIdString;
		if (!registry.TryGetLink(identityString, characterIdString))
			return false;

		UUID linkedCharacterId = characterIdString;
		return !linkedCharacterId.IsNull();
	}

	//------------------------------------------------------------------------------------------------
	protected bool ARGH_IsTrueFirstJoin_S(int playerId, SCR_PlayerController playerController, UUID playerCharacterId = UUID.NULL_UUID, UUID controlledCharacterId = UUID.NULL_UUID)
	{
		return !ARGH_HasPersistedPlayerState_S(playerId, playerController, playerCharacterId, controlledCharacterId);
	}

	//------------------------------------------------------------------------------------------------
	protected bool ARGH_TryGetLinkedCharacterId_S(int playerId, out UUID linkedCharacterId)
	{
		linkedCharacterId = UUID.NULL_UUID;

		ARGH_PlayerCharLinkComponent registry = ARGH_PlayerCharLinkComponent.GetInstance();
		if (!registry)
			return false;

		string identityString;
		if (!ARGH_TryGetIdentityString_S(playerId, identityString))
			return false;

		string characterIdString;
		if (!registry.TryGetLink(identityString, characterIdString))
			return false;

		linkedCharacterId = characterIdString;
		return !linkedCharacterId.IsNull();
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_StartRestoreWindow_S(int playerId)
	{
		if (!ARGH_UsesMercOutRestoreFlow())
			return;

		int token = m_iARGH_NextRestoreToken++;
		m_mARGH_RestoreTimeoutTokens.Set(playerId, token);
		m_aARGH_RestoreSpawnPending.RemoveItem(playerId);
		ARGH_SetRespawnMode_S(playerId, ARGH_RespawnMode.ReconnectRestore);
		ARGH_SetRestorePending_S(playerId, true);
		ARGH_Log(string.Format("player=%1 action=restore-request-started", playerId));
		ARGH_Log(string.Format("player=%1 action=reconnect-classification-persisted", playerId));
		ARGH_Log(string.Format("player=%1 action=restore-timeout-started timeoutMs=%2", playerId, ARGH_RESTORE_TIMEOUT_MS));
		GetGame().GetCallqueue().CallLater(ARGH_OnRestoreTimeout_S, ARGH_RESTORE_TIMEOUT_MS, false, playerId, token);
	}

	//------------------------------------------------------------------------------------------------
	protected bool ARGH_HasRestoreWindow_S(int playerId, int token = -1)
	{
		int activeToken;
		if (!m_mARGH_RestoreTimeoutTokens.Find(playerId, activeToken))
			return false;

		if (token != -1 && activeToken != token)
			return false;

		return true;
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_ClearRestoreWindow_S(int playerId)
	{
		m_mARGH_RestoreTimeoutTokens.Remove(playerId);
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_ClearRestoreState_S(int playerId)
	{
		ARGH_ClearRestoreWindow_S(playerId);
		m_aARGH_RestoreSpawnPending.RemoveItem(playerId);
		ARGH_SetRestorePending_S(playerId, false);
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_ForceFreshPlayerSpawn_S(int playerId, string source)
	{
		ARGH_ClearRestoreState_S(playerId);
		ARGH_RequestForceJoinFaction_S(playerId, string.Format("fresh-spawn-force-%1", source));
		ARGH_SetRespawnMode_S(playerId, ARGH_RespawnMode.ForcedRandomRespawn);
		ARGH_Log(string.Format("player=%1 action=fresh-spawn-force source='%2' mode='ForcedRandomRespawn'", playerId, source));
		ARGH_StartAuditSpawnStallWatchdog_S(playerId, string.Format("fresh-spawn-force-%1", source));

		bool customFlowStarted = ARGH_RandomSpawnLogic.TryStartCustomSpawnFlow_S(playerId, ARGH_JOIN_FORCED_FACTION_KEY);
		ARGH_Log(string.Format("player=%1 action=fresh-spawn-custom-flow started=%2 source='%3' configuredFaction='%4'", playerId, customFlowStarted, source, ARGH_JOIN_FORCED_FACTION_KEY));
		if (customFlowStarted)
			return;

		ARGH_Log(string.Format("player=%1 action=fresh-spawn-fallback source='%2'", playerId, source));
		DoInitialSpawn_S(playerId);
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_StartAuditSpawnStallWatchdog_S(int playerId, string source)
	{
		int existingToken;
		bool hadWatchdog = m_mARGH_AuditSpawnStallTokens.Find(playerId, existingToken);

		int token = m_iARGH_NextAuditStallToken++;
		m_mARGH_AuditSpawnStallTokens.Set(playerId, token);
		if (hadWatchdog)
			ARGH_Log(string.Format("player=%1 action=audit-spawn-watchdog-refresh timeoutMs=%2 source='%3'", playerId, ARGH_AUDIT_SPAWN_STALL_TIMEOUT_MS, source));
		else
			ARGH_Log(string.Format("player=%1 action=audit-spawn-watchdog-start timeoutMs=%2 source='%3'", playerId, ARGH_AUDIT_SPAWN_STALL_TIMEOUT_MS, source));

		GetGame().GetCallqueue().CallLater(ARGH_OnAuditSpawnStallTimeout_S, ARGH_AUDIT_SPAWN_STALL_TIMEOUT_MS, false, playerId, token);
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_ClearAuditSpawnStallWatchdog_S(int playerId)
	{
		m_mARGH_AuditSpawnStallTokens.Remove(playerId);
	}

	//------------------------------------------------------------------------------------------------
	protected bool ARGH_HasAuditSpawnStallWatchdog_S(int playerId, int token = -1)
	{
		int activeToken;
		if (!m_mARGH_AuditSpawnStallTokens.Find(playerId, activeToken))
			return false;

		if (token != -1 && activeToken != token)
			return false;

		return true;
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_OnAuditSpawnStallTimeout_S(int playerId, int token)
	{
		if (!ARGH_UsesMercOutRestoreFlow())
			return;

		if (!ARGH_HasAuditSpawnStallWatchdog_S(playerId, token))
			return;

		SCR_PlayerController playerController = ARGH_GetPlayerController_S(playerId);
		if (!playerController)
		{
			ARGH_ClearAuditSpawnStallWatchdog_S(playerId);
			return;
		}

		if (ARGH_HiddenRandomSpawnService.HasAliveControlledEntity(playerController))
		{
			ARGH_ClearAuditSpawnStallWatchdog_S(playerId);
			ARGH_Log(string.Format("player=%1 action=audit-spawn-watchdog-cleared reason='entity-present'", playerId));
			return;
		}

		ARGH_RespawnMode respawnMode = playerController.ARGH_GetRespawnMode_O();
		ARGH_Log(string.Format("player=%1 action=audit-spawn-stall-timeout mode='%2' restorePending=%3", playerId, typename.EnumToString(ARGH_RespawnMode, respawnMode), playerController.ARGH_IsRestorePending_O()));
		ARGH_ClearAuditSpawnStallWatchdog_S(playerId);
		ARGH_ForceFreshPlayerSpawn_S(playerId, "audit-spawn-stall-timeout");
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_ForceRestoreFallback_S(int playerId, string reason)
	{
		ARGH_Log(string.Format("player=%1 action=restore-fallback reason='%2'", playerId, reason));
		ARGH_ClearRestoreState_S(playerId);
		ARGH_SetRespawnMode_S(playerId, ARGH_RespawnMode.RestoreFallback);
		DoInitialSpawn_S(playerId);
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_OnRestoreTimeout_S(int playerId, int token)
	{
		if (!ARGH_UsesMercOutRestoreFlow())
			return;

		if (!ARGH_HasRestoreWindow_S(playerId, token))
			return;

		SCR_PlayerController playerController = ARGH_GetPlayerController_S(playerId);
		if (!playerController)
		{
			ARGH_ClearRestoreState_S(playerId);
			return;
		}

		if (ARGH_HiddenRandomSpawnService.HasAliveControlledEntity(playerController))
		{
			ARGH_ClearRestoreState_S(playerId);
			return;
		}

		if (ARGH_HasPersistedPlayerState_S(playerId, playerController))
		{
			ARGH_ForceRestoreFallback_S(playerId, "restore-timeout-persisted");
			return;
		}

		ARGH_ForceRestoreFallback_S(playerId, "restore-timeout-no-persisted-state");
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_UpdatePlayerLink_S(int playerId, IEntity entity)
	{
		if (!ARGH_UsesMercOutRestoreFlow() || !m_Persistence || !entity)
			return;

		UUID identityId = SCR_PlayerIdentityUtils.GetPlayerIdentityId(playerId);
		if (identityId.IsNull())
			return;

		UUID characterId = m_Persistence.GetId(entity);
		if (characterId.IsNull())
			return;

		ARGH_PlayerCharLinkComponent registry = ARGH_PlayerCharLinkComponent.GetInstance();
		if (!registry)
			return;

		registry.SetLink(string.Format("%1", identityId), string.Format("%1", characterId));
		ARGH_Log(string.Format("player=%1 action=link-updated identity='%2' character='%3'", playerId, identityId, characterId));
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_SaveDisconnectState_S(int playerId)
	{
		if (!ARGH_UsesMercOutRestoreFlow() || !Replication.IsServer() || !m_Persistence)
			return;

		SCR_PlayerController playerController = ARGH_GetPlayerController_S(playerId);
		if (!playerController)
			return;

		IEntity controlledEntity = playerController.GetControlledEntity();
		IEntity mainEntity = playerController.GetMainEntity();

		IEntity persistentCharacter = controlledEntity;
		if (!persistentCharacter)
			persistentCharacter = mainEntity;

		if (persistentCharacter)
		{
			m_Persistence.Save(persistentCharacter);
			ARGH_Log(string.Format("player=%1 action=disconnect-save-character entity='%2'", playerId, persistentCharacter));
			ARGH_UpdatePlayerLink_S(playerId, persistentCharacter);
		}

		// Save controller after character so serializer can resolve persisted character IDs.
		m_Persistence.Save(playerController);
		ARGH_Log(string.Format("player=%1 action=disconnect-save-controller", playerId));

		DZ_PersistenceCommit.RequestGlobalFlush_S("player_disconnect", ARGH_DISCONNECT_FLUSH_THROTTLE_MS, ARGH_DISCONNECT_FLUSH_DELAY_MS, ESaveGameType.MANUAL);
		ARGH_Log(string.Format("player=%1 action=disconnect-flush-requested", playerId));
	}

	//------------------------------------------------------------------------------------------------
	override void OnPlayerAuditSuccess_S(int playerId)
	{
		if (ARGH_UsesMercOutRestoreFlow())
		{
			ARGH_RequestForceJoinFaction_S(playerId, "audit-success");
			SCR_PlayerController playerController = ARGH_GetPlayerController_S(playerId);
			if (ARGH_HasPersistedPlayerState_S(playerId, playerController))
				ARGH_StartRestoreWindow_S(playerId);
			else if (playerController && ARGH_HiddenRandomSpawnService.HasAliveControlledEntity(playerController))
				ARGH_Log(string.Format("player=%1 action=restore-window-skipped reason='alive-entity-present'", playerId));
			else
			{
				// Defensive classification: avoid stale reconnect flags from previous sessions
				// keeping a new join in an unspawnable state.
				ARGH_ClearRestoreState_S(playerId);
				ARGH_SetRespawnMode_S(playerId, ARGH_RespawnMode.InitialJoin);
				ARGH_Log(string.Format("player=%1 action=player-audit-complete", playerId));
				ARGH_Log(string.Format("player=%1 action=player-audit-classified mode='InitialJoin'", playerId));
				ARGH_StartAuditSpawnStallWatchdog_S(playerId, "audit-success-initial-join");
			}
		}

		super.OnPlayerAuditSuccess_S(playerId);
	}

	//------------------------------------------------------------------------------------------------
	override protected void RequestPlayerData_S(int playerId)
	{
		super.RequestPlayerData_S(playerId);
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnPlayerDataLoaded_S(EPersistenceStatusCode statusCode, Managed result, bool isLast, Managed context)
	{
		auto playerController = SCR_PlayerController.Cast(Tuple1<PlayerController>.Cast(context).param1);
		if (!playerController)
			return;

		int playerId = playerController.GetPlayerId();
		bool useMercOutRestore = ARGH_UsesMercOutRestoreFlow();

		if (ResolveReconnection(playerId))
		{
			ForgetControlledEntityIds(playerController);
			if (useMercOutRestore)
				ARGH_ClearRestoreState_S(playerId);
			return;
		}

		if (statusCode != EPersistenceStatusCode.OK)
		{
			ARGH_Log(string.Format("player=%1 action=player-data-load-non-ok status='%2'", playerId, typename.EnumToString(EPersistenceStatusCode, statusCode)));

			if (useMercOutRestore && statusCode == EPersistenceStatusCode.NOT_FOUND)
			{
				ARGH_StartAuditSpawnStallWatchdog_S(playerId, "player-data-not-found");
				ARGH_ForceFreshPlayerSpawn_S(playerId, "player-data-not-found");
				return;
			}

			if (useMercOutRestore && ARGH_HasRestoreWindow_S(playerId))
			{
				ARGH_Log(string.Format("player=%1 action=restore-load-wait status='%2'", playerId, typename.EnumToString(EPersistenceStatusCode, statusCode)));
				return;
			}

			if (useMercOutRestore)
				ARGH_StartAuditSpawnStallWatchdog_S(playerId, "player-data-load-non-ok");
			DoInitialSpawn_S(playerId);
			return;
		}

		if (playerController != result)
			return;

		UUID playerCharacterId, controlledCharacterId;
		ConsumeControlledEntityIds(playerController, playerCharacterId, controlledCharacterId);

		if (playerCharacterId.IsNull())
		{
			if (!controlledCharacterId.IsNull())
			{
				if (useMercOutRestore)
				{
					ARGH_ClearRestoreWindow_S(playerId);
					ARGH_SetRespawnMode_S(playerId, ARGH_RespawnMode.ReconnectRestore);
				}

				Tuple2<int, bool> characterAvailableContext(playerController.GetPlayerId(), true);
				PersistenceWhenAvailableTask linkControlledEntityTask(OnControlledCharacterAvailable, characterAvailableContext);
				m_Persistence.WhenAvailable(controlledCharacterId, linkControlledEntityTask);
				return;
			}

			if (useMercOutRestore)
			{
				if (ARGH_IsTrueFirstJoin_S(playerId, playerController, playerCharacterId, controlledCharacterId))
				{
					ARGH_Log(string.Format("player=%1 action=reconnect-classification-new-player", playerId));
					ARGH_Log(string.Format("player=%1 action=first-time-player-immediate-spawn", playerId));
					ARGH_StartAuditSpawnStallWatchdog_S(playerId, "fresh-save-or-player-not-found");
					ARGH_ForceFreshPlayerSpawn_S(playerId, "fresh-save-or-player-not-found");
					return;
				}
				else
				{
					UUID linkedCharacterId;
					if (ARGH_TryGetLinkedCharacterId_S(playerId, linkedCharacterId))
					{
						ARGH_ClearRestoreWindow_S(playerId);
						ARGH_SetRespawnMode_S(playerId, ARGH_RespawnMode.ReconnectRestore);

						PersistenceSpawnRequest linkedRequest();
						linkedRequest.Collection = m_CharacterCollection;
						linkedRequest.Include = {linkedCharacterId};

						Tuple1<int> linkedContext(playerId);
						PersistenceResultCallback linkedCallback(OnPlayerCharacterLoaded_S, linkedContext);
						m_Persistence.RequestSpawn(linkedRequest, linkedCallback);
						ARGH_Log(string.Format("player=%1 action=linked-character-load-requested character='%2'", playerId, linkedCharacterId));
						return;
					}

					ARGH_ForceRestoreFallback_S(playerId, "reconnect-missing-character-id");
					return;
				}
			}

			DoInitialSpawn_S(playerId);
			if (useMercOutRestore)
				ARGH_StartAuditSpawnStallWatchdog_S(playerId, "player-data-no-character-id");
			return;
		}

		if (!controlledCharacterId.IsNull() && controlledCharacterId != playerCharacterId)
			m_mPendingPosessions.Set(playerId, controlledCharacterId);

		if (useMercOutRestore)
		{
			ARGH_ClearRestoreWindow_S(playerId);
			ARGH_SetRespawnMode_S(playerId, ARGH_RespawnMode.ReconnectRestore);
		}

		PersistenceSpawnRequest request();
		request.Collection = m_CharacterCollection;
		request.Include = {playerCharacterId};

		Tuple1<int> playerCharContext(playerId);
		PersistenceResultCallback callback(OnPlayerCharacterLoaded_S, playerCharContext);
		m_Persistence.RequestSpawn(request, callback);
		ARGH_Log(string.Format("player=%1 action=saved-character-load-requested character='%2'", playerId, playerCharacterId));
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnPlayerCharacterLoaded_S(EPersistenceStatusCode statusCode, Managed result, bool isLast, Managed context)
	{
		auto playerDataContext = Tuple1<int>.Cast(context);
		int playerId = playerDataContext.param1;
		bool useMercOutRestore = ARGH_UsesMercOutRestoreFlow();

		BaseGameEntity player = BaseGameEntity.Cast(result);

		#ifdef WORKBENCH
		if (player)
		{
			bool needsChange;
			vector transform[4];
			player.GetWorldTransform(transform);

			if (s_vPlayFromCameraPos != vector.Zero)
			{
				transform[3] = s_vPlayFromCameraPos;
				needsChange = true;
				s_vPlayFromCameraPos = vector.Zero;
			}

			if (s_vPlayFromCameraYpr != vector.Zero)
			{
				Math3D.AnglesToMatrix(s_vPlayFromCameraYpr, transform);
				needsChange = true;
				s_vPlayFromCameraYpr = vector.Zero;
			}

			if (needsChange)
				player.Teleport(transform);
		}

		PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(playerId);
		IEntity controlledEntity = playerController.GetControlledEntity();
		SCR_EntityHelper.DeleteEntityAndChildren(controlledEntity);
		#endif

		bool savedCharacterDead;
		if (player)
		{
			CharacterControllerComponent charController = CharacterControllerComponent.Cast(player.FindComponent(CharacterControllerComponent));
			if (charController && charController.GetLifeState() == ECharacterLifeState.DEAD)
			{
				savedCharacterDead = true;
				player = null;
			}
		}

		if (statusCode != EPersistenceStatusCode.OK || !player)
		{
			if (useMercOutRestore)
			{
				ARGH_ClearRestoreState_S(playerId);
				if (savedCharacterDead)
				{
					ARGH_Log(string.Format("player=%1 action=reconnect-classification-dead-character", playerId));
					ARGH_Log(string.Format("player=%1 action=saved-character-dead-countdown", playerId));
					ARGH_SetRespawnMode_S(playerId, ARGH_RespawnMode.DeathRespawn);
				}
					else
					{
						ARGH_ForceRestoreFallback_S(playerId, string.Format("restore-failed status='%1'", typename.EnumToString(EPersistenceStatusCode, statusCode)));
						return;
					}
				}

			DoInitialSpawn_S(playerId);
			return;
		}

		auto data = SCR_PossessSpawnData.FromEntity(player);
		if (useMercOutRestore)
		{
			m_aARGH_RestoreSpawnPending.Insert(playerId);
			ARGH_Log(string.Format("player=%1 action=saved-character-restore-request", playerId));
		}

		GetPlayerRespawnComponent_S(playerId).RequestSpawn(data);
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnControlledCharacterAvailable(Managed instance, PersistenceDeferredDeserializeTask task, bool expired, Managed context)
	{
		auto characterAvailableContext = Tuple2<int, bool>.Cast(context);
		int playerId = characterAvailableContext.param1;
		bool useMercOutRestore = ARGH_UsesMercOutRestoreFlow();

		IEntity entity = IEntity.Cast(instance);
		if (entity)
		{
			CharacterControllerComponent charController = CharacterControllerComponent.Cast(entity.FindComponent(CharacterControllerComponent));
			if (charController && charController.GetLifeState() != ECharacterLifeState.DEAD)
			{
				SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
				playerController.SetPossessedEntity(entity);
				ARGH_Log(string.Format("player=%1 action=restore-entity-possessed", playerId));
				return;
			}
		}

		if (characterAvailableContext.param2)
		{
				if (useMercOutRestore)
				{
					ARGH_ForceRestoreFallback_S(playerId, "controlled-entity-unavailable");
					return;
				}

			DoInitialSpawn_S(playerId);
		}
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnPlayerSpawnFailed_S(int playerId)
	{
		if (ARGH_UsesMercOutRestoreFlow() && m_aARGH_RestoreSpawnPending.Contains(playerId))
		{
			ARGH_ForceRestoreFallback_S(playerId, "restore-spawn-rejected");
			return;
		}

		super.OnPlayerSpawnFailed_S(playerId);
	}

	//------------------------------------------------------------------------------------------------
	override void OnPlayerSpawned_S(int playerId, IEntity entity)
	{
		super.OnPlayerSpawned_S(playerId, entity);

		if (!ARGH_UsesMercOutRestoreFlow())
			return;

		ARGH_UpdatePlayerLink_S(playerId, entity);
	}

	//------------------------------------------------------------------------------------------------
	override void OnPlayerEntityChanged_S(int playerId, IEntity previousEntity, IEntity newEntity)
	{
		super.OnPlayerEntityChanged_S(playerId, previousEntity, newEntity);

		if (!ARGH_UsesMercOutRestoreFlow())
			return;

		SCR_PlayerController playerController = ARGH_GetPlayerController_S(playerId);

		if (newEntity)
		{
			ARGH_ClearAuditSpawnStallWatchdog_S(playerId);
			ARGH_UpdatePlayerLink_S(playerId, newEntity);

			if (m_aARGH_RestoreSpawnPending.Contains(playerId))
			{
				ARGH_ClearRestoreState_S(playerId);
				ARGH_SetRespawnMode_S(playerId, ARGH_RespawnMode.None);
				if (playerController)
					playerController.ARGH_SetDeathReason_S(ARGH_DeathReason.Unknown);
				ARGH_Log(string.Format("player=%1 action=saved-character-restored", playerId));
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnPlayerEntityLost_S(int playerId)
	{
		if (ARGH_UsesMercOutRestoreFlow())
		{
			ARGH_ClearRestoreState_S(playerId);
			SCR_PlayerController playerController = ARGH_GetPlayerController_S(playerId);
			if (playerController)
			{
				if (ARGH_HiddenRandomSpawnService.HasAliveControlledEntity(playerController))
				{
					ARGH_Log(string.Format("player=%1 action=entity-lost-ignored reason='alive-entity-present'", playerId));
					super.OnPlayerEntityLost_S(playerId);
					return;
				}

				if (playerController.ARGH_GetRespawnMode_O() == ARGH_RespawnMode.ReconnectRestore)
				{
					if (playerController.ARGH_IsRestorePending_O())
					{
						ARGH_Log(string.Format("player=%1 action=entity-lost-ignored reason='restore-pending'", playerId));
						super.OnPlayerEntityLost_S(playerId);
						return;
					}

					ARGH_Log(string.Format("player=%1 action=reconnect-restore-death-fallback", playerId));
					ARGH_SetRespawnMode_S(playerId, ARGH_RespawnMode.DeathRespawn);
				}
			}

			if (playerController && playerController.ARGH_GetRespawnMode_O() == ARGH_RespawnMode.ForcedRandomRespawn)
			{
				playerController.ARGH_SetDeathReason_S(ARGH_DeathReason.Suicide);
				ARGH_Log(string.Format("player=%1 action=forced-random-respawn-requested", playerId));
			}
			else
			{
				if (playerController && playerController.ARGH_GetDeathReason_O() == ARGH_DeathReason.Unknown)
					playerController.ARGH_SetDeathReason_S(ARGH_DeathReason.Death);

				ARGH_SetRespawnMode_S(playerId, ARGH_RespawnMode.DeathRespawn);
				ARGH_Log(string.Format("player=%1 action=death-countdown-armed", playerId));
			}
		}

		super.OnPlayerEntityLost_S(playerId);
	}

	//------------------------------------------------------------------------------------------------
	override void OnPlayerDisconnected_S(int playerId, KickCauseCode cause, int timeout)
	{
		if (ARGH_UsesMercOutRestoreFlow())
		{
			ARGH_SaveDisconnectState_S(playerId);

			m_mARGH_RestoreTimeoutTokens.Remove(playerId);
			m_mARGH_AuditSpawnStallTokens.Remove(playerId);
			m_mARGH_JoinFactionTokens.Remove(playerId);
			m_aARGH_RestoreSpawnPending.RemoveItem(playerId);
		}

		super.OnPlayerDisconnected_S(playerId, cause, timeout);
	}

	//------------------------------------------------------------------------------------------------
	void ARGH_RequestPauseExitSave_S(int playerId)
	{
		if (!ARGH_UsesMercOutRestoreFlow() || !Replication.IsServer() || !m_Persistence)
			return;

		SCR_PlayerController playerController = ARGH_GetPlayerController_S(playerId);
		if (!playerController)
			return;

		IEntity controlledEntity = playerController.GetControlledEntity();
		IEntity mainEntity = playerController.GetMainEntity();
		IEntity persistentCharacter = controlledEntity;
		if (!persistentCharacter)
			persistentCharacter = mainEntity;

		if (persistentCharacter)
		{
			m_Persistence.Save(persistentCharacter);
			ARGH_Log(string.Format("player=%1 action=pause-exit-save-character entity='%2'", playerId, persistentCharacter));
			ARGH_UpdatePlayerLink_S(playerId, persistentCharacter);
		}

		// Save controller after character so serializer can resolve persisted character IDs.
		m_Persistence.Save(playerController);
		ARGH_Log(string.Format("player=%1 action=pause-exit-save-controller", playerId));

		SCR_PersistenceSystem scriptedPersistence = SCR_PersistenceSystem.GetScriptedInstance();
		if (scriptedPersistence && scriptedPersistence.GetState() == EPersistenceSystemState.ACTIVE)
		{
			if (scriptedPersistence.TriggerSave(ESaveGameType.MANUAL))
				ARGH_Log(string.Format("player=%1 action=pause-exit-trigger-save-requested", playerId));
			else
				ARGH_Log(string.Format("player=%1 action=pause-exit-trigger-save-not-started", playerId));
		}

		SaveGameManager saveManager = GetGame().GetSaveGameManager();
		if (!saveManager || !saveManager.IsSavingAllowed() || !saveManager.IsSavingPossible() || saveManager.IsBusy())
		{
			ARGH_Log(string.Format("player=%1 action=pause-exit-overwrite-failed", playerId));
			return;
		}

		ARGH_Log(string.Format("player=%1 action=pause-exit-overwrite-requested", playerId));
		if (DZ_SaveGameUtil.OverwriteLatestSave(saveManager))
			ARGH_Log(string.Format("player=%1 action=pause-exit-overwrite-succeeded", playerId));
		else
			ARGH_Log(string.Format("player=%1 action=pause-exit-overwrite-failed", playerId));
	}
}

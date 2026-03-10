class ARGH_HiddenRandomSpawnService
{
	static const ResourceName ARGH_RANDOM_SPAWN_CONFIG = "{2A77B2B7CF6E4FA1}Configs/Systems/Respawn/ARGH_RandomSpawn.conf";
	static const FactionKey MERCOUT_RUNTIME_FACTION = "PLAYERS";
	static const int MAX_RETRY_BEFORE_SERVER_ESCALATION = 8;

	protected static ref ARGH_RandomSpawnConfig s_Config;
	protected static bool s_bConfigLoaded;
	protected static ref map<int, ref array<string>> s_PlayerRecentSpawns = new map<int, ref array<string>>();
	protected static ref set<int> s_PendingRetries = new set<int>();
	protected static ref set<int> s_InFlightSpawnRequests = new set<int>();
	protected static ref map<int, int> s_RetryCounts = new map<int, int>();

	//------------------------------------------------------------------------------------------------
	static bool IsActive()
	{
		ARGH_RandomSpawnConfig config = GetConfig();
		if (!config || !config.Enabled)
			return false;

		return IsMercOutScenario();
	}

	//------------------------------------------------------------------------------------------------
	static bool HasPendingLocalRequest()
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!playerController)
			return false;

		int playerId = playerController.GetPlayerId();
		return s_InFlightSpawnRequests.Contains(playerId) || s_PendingRetries.Contains(playerId);
	}

	//------------------------------------------------------------------------------------------------
	static bool TryRequestLocalSpawn()
	{
		if (!IsActive())
			return false;

		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!playerController)
			return false;

		int playerId = playerController.GetPlayerId();
		if (playerController.ARGH_IsRestorePending_O())
		{
			Print(string.Format("[ARGH_PERSIST] player=%1 action=skip-fallback-spawn reason='restore-pending'", playerId), LogLevel.WARNING);
			return false;
		}

		ARGH_RespawnMode respawnMode = playerController.ARGH_GetRespawnMode_O();
		if (respawnMode == ARGH_RespawnMode.None || respawnMode == ARGH_RespawnMode.ReconnectRestore)
		{
			Print(string.Format("[ARGH_PERSIST] player=%1 action=skip-fallback-spawn reason='respawn-mode-%2'", playerId, typename.EnumToString(ARGH_RespawnMode, respawnMode)), LogLevel.WARNING);
			return false;
		}

		if (HasAliveControlledEntity(playerController))
		{
			ClearPendingState(playerId);
			return false;
		}

		if (s_InFlightSpawnRequests.Contains(playerId))
		{
			Print(string.Format("[ARGH_AUTODEPLOY] player=%1 action=skip reason='spawn-request-in-flight'", playerId), LogLevel.WARNING);
			return true;
		}

		Print(string.Format("[ARGH_AUTODEPLOY] player=%1 action=start", playerId), LogLevel.WARNING);
		SCR_RespawnComponent respawnComponent = SCR_RespawnComponent.GetInstance();
		if (!respawnComponent)
		{
			ScheduleRetry(playerId, "respawn-component-missing");
			return true;
		}

		ResourceName loadoutResource;
		string loadoutFailureReason;
		if (!TryResolveLoadoutResource(playerController, loadoutResource, loadoutFailureReason))
		{
			ScheduleRetry(playerId, loadoutFailureReason);
			return true;
		}

		ARGH_RandomSpawnConfig config = GetConfig();
		array<SCR_SpawnPoint> candidates = {};
		CollectCandidates(config, candidates);
		if (candidates.IsEmpty())
		{
			ScheduleRetry(playerId, "no-allowed-coastal-spawns");
			return true;
		}

		array<SCR_SpawnPoint> filteredCandidates = {};
		FilterByRecentHistory(playerId, candidates, filteredCandidates, config.AvoidLastNPerPlayer);
		if (filteredCandidates.IsEmpty())
			filteredCandidates = candidates;

		array<SCR_SpawnPoint> attemptPool = {};
		foreach (SCR_SpawnPoint spawnPoint : filteredCandidates)
		{
			if (spawnPoint)
				attemptPool.Insert(spawnPoint);
		}

		while (!attemptPool.IsEmpty())
		{
			int index = Math.RandomInt(0, attemptPool.Count());
			SCR_SpawnPoint chosenSpawn = attemptPool[index];
			attemptPool.Remove(index);
			if (!chosenSpawn)
				continue;

			string spawnName = chosenSpawn.GetSpawnPointName();
			string spawnHistoryKey = GetSpawnHistoryKey(chosenSpawn);
			SCR_SpawnPointSpawnData spawnData = new SCR_SpawnPointSpawnData(loadoutResource, chosenSpawn.GetRplId());

			if (!respawnComponent.CanSpawn(spawnData))
			{
				Print(string.Format("[ARGH_AUTODEPLOY] player=%1 candidate='%2' action=can-spawn-denied", playerId, spawnName), LogLevel.WARNING);
				continue;
			}

			if (!respawnComponent.RequestSpawn(spawnData))
			{
				Print(string.Format("[ARGH_AUTODEPLOY] player=%1 candidate='%2' action=request-spawn-failed", playerId, spawnName), LogLevel.WARNING);
				continue;
			}

			s_PendingRetries.RemoveItem(playerId);
			s_RetryCounts.Remove(playerId);
			s_InFlightSpawnRequests.Insert(playerId);
			TrackHistory(playerId, spawnHistoryKey, config.AvoidLastNPerPlayer);
			Print(string.Format("[ARGH_AUTODEPLOY] player=%1 action=request-spawn spawn='%2'", playerId, spawnName), LogLevel.WARNING);
			return true;
		}

		ScheduleRetry(playerId, "all-coastal-spawns-rejected");
		return true;
	}

	//------------------------------------------------------------------------------------------------
	protected static bool TryResolveLoadoutResource(SCR_PlayerController playerController, out ResourceName loadoutResource, out string failureReason)
	{
		loadoutResource = string.Empty;
		failureReason = string.Empty;

		if (!playerController)
		{
			failureReason = "player-controller-missing";
			return false;
		}

		SCR_PlayerFactionAffiliationComponent factionComponent = SCR_PlayerFactionAffiliationComponent.Cast(playerController.FindComponent(SCR_PlayerFactionAffiliationComponent));
		SCR_PlayerLoadoutComponent loadoutComponent = SCR_PlayerLoadoutComponent.Cast(playerController.FindComponent(SCR_PlayerLoadoutComponent));
		if (!factionComponent || !loadoutComponent)
		{
			failureReason = "player-components-pending";
			return false;
		}

		ARGH_RandomSpawnConfig config = GetConfig();
		Faction faction = ResolveSpawnFaction(config.AllowedFactionKey);
		if (!faction)
		{
			failureReason = string.Format("faction-missing key=%1", config.AllowedFactionKey);
			return false;
		}

		factionComponent.RequestFaction(faction);

		SCR_BasePlayerLoadout loadout = loadoutComponent.GetLoadout();
		if (!loadout)
		{
			SCR_LoadoutManager loadoutManager = GetGame().GetLoadoutManager();
			if (!loadoutManager)
			{
				failureReason = "loadout-manager-missing";
				return false;
			}

			loadout = loadoutManager.GetPlayerLoadout(playerController.GetPlayerId());
			if (loadout)
				loadoutComponent.RequestLoadout(loadout);

			if (!loadout)
			{
				array<ref SCR_BasePlayerLoadout> factionLoadouts = {};
				if (loadoutManager.GetPlayerLoadoutsByFaction(faction, factionLoadouts) > 0)
					loadout = factionLoadouts[0];

				if (loadout)
					loadoutComponent.RequestLoadout(loadout);
			}
		}

		if (!loadout)
		{
			loadout = loadoutComponent.GetLoadout();
		}

		if (!loadout)
		{
			failureReason = string.Format("loadout-unavailable faction=%1", faction.GetFactionKey());
			return false;
		}

		loadoutResource = loadout.GetLoadoutResource();
		if (loadoutResource.IsEmpty())
		{
			failureReason = string.Format("loadout-resource-empty loadout=%1", loadout.GetLoadoutName());
			return false;
		}

		return true;
	}

	//------------------------------------------------------------------------------------------------
	protected static void CollectCandidates(ARGH_RandomSpawnConfig config, out array<SCR_SpawnPoint> outCandidates)
	{
		int spawnCount = SCR_SpawnPoint.CountSpawnPoints();
		for (int i = 0; i < spawnCount; i++)
		{
			SCR_SpawnPoint spawnPoint = SCR_SpawnPoint.GetSpawnPointByIndex(i);
			if (!spawnPoint)
				continue;

			if (!spawnPoint.IsSpawnPointEnabled() || !spawnPoint.IsSpawnPointActive())
				continue;

			if (!IsAllowedSpawnName(config, spawnPoint.GetSpawnPointName()))
				continue;

			outCandidates.Insert(spawnPoint);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected static bool IsAllowedSpawnName(ARGH_RandomSpawnConfig config, string spawnName)
	{
		if (!config || !config.AllowedSpawnNames || config.AllowedSpawnNames.IsEmpty())
			return true;

		foreach (string allowedName : config.AllowedSpawnNames)
		{
			if (allowedName == spawnName)
				return true;
		}

		return false;
	}

	//------------------------------------------------------------------------------------------------
	protected static void FilterByRecentHistory(int playerId, array<SCR_SpawnPoint> source, out array<SCR_SpawnPoint> filtered, int avoidLastN)
	{
		if (avoidLastN <= 0)
		{
			foreach (SCR_SpawnPoint spawnPoint : source)
			{
				if (spawnPoint)
					filtered.Insert(spawnPoint);
			}
			return;
		}

		array<string> history = GetRecentHistory(playerId);
		foreach (SCR_SpawnPoint spawnPoint : source)
		{
			if (!spawnPoint)
				continue;

			string historyKey = GetSpawnHistoryKey(spawnPoint);
			if (history.Find(historyKey) == -1)
				filtered.Insert(spawnPoint);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected static array<string> GetRecentHistory(int playerId)
	{
		array<string> history;
		if (!s_PlayerRecentSpawns.Find(playerId, history))
			return new array<string>();

		return history;
	}

	//------------------------------------------------------------------------------------------------
	protected static void TrackHistory(int playerId, string spawnHistoryKey, int keepCount)
	{
		if (keepCount <= 0 || spawnHistoryKey.IsEmpty())
			return;

		array<string> history;
		if (!s_PlayerRecentSpawns.Find(playerId, history))
		{
			history = {};
			s_PlayerRecentSpawns.Insert(playerId, history);
		}

		history.Insert(spawnHistoryKey);
		while (history.Count() > keepCount)
			history.Remove(0);
	}

	//------------------------------------------------------------------------------------------------
	protected static string GetSpawnHistoryKey(SCR_SpawnPoint spawnPoint)
	{
		if (!spawnPoint)
			return string.Empty;

		return string.Format("%1|%2", spawnPoint.GetSpawnPointName(), spawnPoint.GetRplId());
	}

	//------------------------------------------------------------------------------------------------
	protected static void ScheduleRetry(int playerId, string reason)
	{
		if (s_InFlightSpawnRequests.Contains(playerId))
			return;

		if (s_PendingRetries.Contains(playerId))
			return;

		int retryCount = 0;
		s_RetryCounts.Find(playerId, retryCount);
		retryCount++;
		s_RetryCounts.Set(playerId, retryCount);

		if (retryCount >= MAX_RETRY_BEFORE_SERVER_ESCALATION)
		{
			SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
			if (playerController && playerController.GetPlayerId() == playerId)
			{
				// Escalate to server-side spawn reservation path when local CanSpawn loops.
				playerController.ARGH_RequestSpawnDialogAction(true);
				Print(string.Format("[ARGH_AUTODEPLOY] player=%1 action=server-escalation attempts=%2 reason='%3'", playerId, retryCount, reason), LogLevel.WARNING);
				return;
			}
		}

		ScriptCallQueue callQueue = GetGame().GetCallqueue();
		if (!callQueue)
			return;

		s_PendingRetries.Insert(playerId);
		Print(string.Format("[ARGH_AUTODEPLOY] player=%1 action=retry reason='%2'", playerId, reason), LogLevel.WARNING);
		callQueue.CallLater(ExecuteRetry, 400, false, playerId);
	}

	//------------------------------------------------------------------------------------------------
	protected static void ExecuteRetry(int playerId)
	{
		s_PendingRetries.RemoveItem(playerId);

		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!playerController || playerController.GetPlayerId() != playerId)
			return;

		TryRequestLocalSpawn();
	}

	//------------------------------------------------------------------------------------------------
	static void NotifyLocalControlledEntityChanged(SCR_PlayerController playerController, IEntity controlledEntity)
	{
		if (!playerController)
			return;

		int playerId = playerController.GetPlayerId();
		if (controlledEntity && HasAliveControlledEntity(playerController))
		{
			ClearPendingState(playerId);
			ARGH_SpawnDialog.CloseCurrent();
			Print(string.Format("[ARGH_AUTODEPLOY] player=%1 action=spawn-complete", playerId), LogLevel.WARNING);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected static void ClearPendingState(int playerId)
	{
		s_PendingRetries.RemoveItem(playerId);
		s_InFlightSpawnRequests.RemoveItem(playerId);
		s_RetryCounts.Remove(playerId);
	}

	//------------------------------------------------------------------------------------------------
	static bool HasAliveControlledEntity(SCR_PlayerController playerController)
	{
		if (!playerController)
			return false;

		IEntity controlledEntity = playerController.GetControlledEntity();
		if (!controlledEntity || controlledEntity.IsDeleted())
			return false;

		ChimeraCharacter character = ChimeraCharacter.Cast(controlledEntity);
		if (!character)
			return false;

		CharacterControllerComponent characterController = character.GetCharacterController();
		if (!characterController)
			return false;

		return !characterController.IsDead();
	}

	//------------------------------------------------------------------------------------------------
	protected static Faction ResolveFaction(string factionKey)
	{
		if (factionKey.IsEmpty())
			return null;

		FactionKey resolvedFactionKey = factionKey;
		SCR_FactionAliasComponent aliasComponent = SCR_FactionAliasComponent.Cast(GetGame().GetFactionManager().FindComponent(SCR_FactionAliasComponent));
		if (aliasComponent)
			resolvedFactionKey = aliasComponent.ResolveFactionAlias(resolvedFactionKey);

		return GetGame().GetFactionManager().GetFactionByKey(resolvedFactionKey);
	}

	//------------------------------------------------------------------------------------------------
	protected static Faction ResolveSpawnFaction(string configuredFactionKey)
	{
		Faction faction = ResolveFaction(configuredFactionKey);
		if (faction)
			return faction;

		SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return null;

		Faction localFaction = factionManager.GetLocalPlayerFaction();
		if (localFaction)
		{
			Print(string.Format("[ARGH_AUTODEPLOY] action=faction-fallback configured='%1' resolved='%2'", configuredFactionKey, localFaction.GetFactionKey()), LogLevel.WARNING);
			return localFaction;
		}

		Faction mercOutFaction = ResolveFaction(MERCOUT_RUNTIME_FACTION);
		if (mercOutFaction)
		{
			Print(string.Format("[ARGH_AUTODEPLOY] action=faction-fallback configured='%1' resolved='%2'", configuredFactionKey, MERCOUT_RUNTIME_FACTION), LogLevel.WARNING);
			return mercOutFaction;
		}

		array<Faction> factions = {};
		factionManager.GetFactionsList(factions);
		foreach (Faction candidate : factions)
		{
			SCR_Faction scrFaction = SCR_Faction.Cast(candidate);
			if (scrFaction && scrFaction.IsPlayable())
			{
				Print(string.Format("[ARGH_AUTODEPLOY] action=faction-fallback configured='%1' resolved='%2'", configuredFactionKey, candidate.GetFactionKey()), LogLevel.WARNING);
				return candidate;
			}
		}

		return null;
	}

	//------------------------------------------------------------------------------------------------
	static bool IsMercOutScenario()
	{
		ChimeraGame game = GetGame();
		if (!game)
			return false;

		SCR_MissionHeader missionHeader = SCR_MissionHeader.Cast(game.GetMissionHeader());
		if (missionHeader)
		{
			if (missionHeader.m_sName == "ARGH SURVIVORS")
				return true;

			if (missionHeader.GetWorldPath().Contains("MO_Kolguyev_ARGH.ent"))
				return true;
		}

		return game.GetWorldFile().Contains("MO_Kolguyev_ARGH.ent");
	}

	//------------------------------------------------------------------------------------------------
	protected static ARGH_RandomSpawnConfig GetConfig()
	{
		if (s_bConfigLoaded)
			return s_Config;

		s_bConfigLoaded = true;
		s_Config = CreateDefaultConfig();

		Resource configResource = Resource.Load(ARGH_RANDOM_SPAWN_CONFIG);
		if (!configResource || !configResource.IsValid())
			return s_Config;

		BaseResourceObject resourceObject = configResource.GetResource();
		if (!resourceObject)
			return s_Config;

		BaseContainer container = resourceObject.ToBaseContainer();
		if (!container)
			return s_Config;

		Managed managedConfig = BaseContainerTools.CreateInstanceFromContainer(container);
		ARGH_RandomSpawnConfig loadedConfig = ARGH_RandomSpawnConfig.Cast(managedConfig);
		if (!loadedConfig)
			return s_Config;

		if (!loadedConfig.AllowedSpawnNames)
			loadedConfig.AllowedSpawnNames = new array<string>();

		s_Config = loadedConfig;
		return s_Config;
	}

	//------------------------------------------------------------------------------------------------
	protected static ARGH_RandomSpawnConfig CreateDefaultConfig()
	{
		ARGH_RandomSpawnConfig config = new ARGH_RandomSpawnConfig();
		config.Enabled = true;
		config.AllowedFactionKey = "PLAYERS";
		config.AvoidLastNPerPlayer = 2;
		config.FallbackToMenuOnEmptyPool = false;
		config.UseSpawnPointRandomRadius = true;
		config.AllowedSpawnNames = {};
		return config;
	}
}

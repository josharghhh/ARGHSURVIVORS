[BaseContainerProps(configRoot: true)]
class ARGH_RandomSpawnConfig : Managed
{
	[Attribute(defvalue: "1", uiwidget: UIWidgets.CheckBox, desc: "Enable randomized spawn logic.")]
	bool Enabled;

	[Attribute(desc: "Allowed spawn point names.")]
	ref array<string> AllowedSpawnNames;

	[Attribute(defvalue: "PLAYERS", uiwidget: UIWidgets.EditBox, desc: "Allowed faction key.")]
	string AllowedFactionKey;

	[Attribute(defvalue: "2", uiwidget: UIWidgets.EditBox, desc: "Avoid this many recent spawn names per player.")]
	int AvoidLastNPerPlayer;

	[Attribute(defvalue: "1", uiwidget: UIWidgets.CheckBox, desc: "If pool is invalid, fallback to vanilla menu flow.")]
	bool FallbackToMenuOnEmptyPool;

	[Attribute(defvalue: "1", uiwidget: UIWidgets.CheckBox, desc: "Use per-point random radius.")]
	bool UseSpawnPointRandomRadius;
}

[BaseContainerProps(category: "Respawn")]
class ARGH_RandomSpawnLogic : SCR_AutoSpawnLogic
{
	static const ResourceName ARGH_RANDOM_SPAWN_CONFIG = "{2A77B2B7CF6E4FA1}Configs/Systems/Respawn/ARGH_RandomSpawn.conf";

	protected static ref ARGH_RandomSpawnConfig s_Config;
	protected static bool s_bConfigLoaded;
	protected static ARGH_RandomSpawnLogic s_Instance;
	protected static ref map<int, ref array<string>> s_PlayerRecentSpawns = new map<int, ref array<string>>();
	protected static ref set<int> s_PendingSpawnRetries = new set<int>();
	protected static const FactionKey ARGH_RUNTIME_FALLBACK_FACTION = "PLAYERS";

	protected string m_sLastFallbackReason;

	//------------------------------------------------------------------------------------------------
	void ARGH_RandomSpawnLogic()
	{
		s_Instance = this;
	}

	//------------------------------------------------------------------------------------------------
	static ARGH_RandomSpawnLogic GetInstance()
	{
		return s_Instance;
	}

	//------------------------------------------------------------------------------------------------
	static bool ShouldUseCustomSpawnFlow()
	{
		ARGH_RandomSpawnConfig config = GetConfig();
		if (!config || !config.Enabled)
			return false;

		if (IsMercOutScenario())
			return true;

		return HasConfiguredSpawnPoints(config);
	}

	//------------------------------------------------------------------------------------------------
	static bool TryStartCustomSpawnFlow_S(int playerId, string configuredFactionKey = "")
	{
		Print(string.Format("[ARGH_TRACE] ARGH_RandomSpawnLogic.TryStartCustomSpawnFlow_S player=%1 configuredFaction=%2", playerId, configuredFactionKey), LogLevel.WARNING);

		ARGH_RandomSpawnConfig config = GetConfig();
		if (!config || !config.Enabled)
			return false;

		if (!IsMercOutScenario() && !HasConfiguredSpawnPoints(config))
			return false;

		string factionKey = configuredFactionKey;
		if (factionKey.IsEmpty())
			factionKey = config.AllowedFactionKey;

		array<SCR_SpawnPoint> candidates = {};
		CollectCandidatesStatic(playerId, config, candidates, true);
		int fullPoolCount = candidates.Count();

		if (fullPoolCount <= 0)
		{
			ScheduleSpawnRetryStatic(playerId, "no-eligible-coastal-spawns");
			FallbackToCustomDialogStatic(playerId, "No coastal spawn available", "No valid coastal survivor spawn is currently available. Retrying automatically.");
			return true;
		}

		array<SCR_SpawnPoint> filteredCandidates = {};
		FilterByRecentHistoryStatic(playerId, candidates, filteredCandidates, config.AvoidLastNPerPlayer);
		if (filteredCandidates.IsEmpty())
			filteredCandidates = candidates;

		SCR_RespawnComponent respawnComponent = GetPlayerRespawnComponentStatic(playerId);
		if (!respawnComponent)
		{
			ScheduleSpawnRetryStatic(playerId, "respawn-component-missing");
			return true;
		}

		ResourceName loadoutResource;
		string loadoutFailureReason;
		if (!TryResolveSpawnLoadoutResourceStatic(playerId, factionKey, loadoutResource, loadoutFailureReason))
		{
			ScheduleSpawnRetryStatic(playerId, loadoutFailureReason);
			return true;
		}

		array<SCR_SpawnPoint> attemptPool = {};
		foreach (SCR_SpawnPoint candidate : filteredCandidates)
		{
			if (candidate)
				attemptPool.Insert(candidate);
		}

		while (!attemptPool.IsEmpty())
		{
			int index = Math.RandomInt(0, attemptPool.Count());
			SCR_SpawnPoint chosenSpawn = attemptPool[index];
			attemptPool.Remove(index);
			if (!chosenSpawn)
				continue;

			string spawnName = chosenSpawn.GetSpawnPointName();
			string spawnHistoryKey = GetSpawnHistoryKeyStatic(chosenSpawn);
			SCR_SpawnPointSpawnData spawnData = new SCR_SpawnPointSpawnData(loadoutResource, chosenSpawn.GetRplId());

			if (!respawnComponent.CanSpawn(spawnData))
			{
				Print(string.Format("[ARGH_SPAWN] player=%1 candidate='%2' action=can-spawn-denied loadout=%3", playerId, spawnName, loadoutResource), LogLevel.NORMAL);
				continue;
			}

			if (!respawnComponent.RequestSpawn(spawnData))
			{
				Print(string.Format("[ARGH_SPAWN] player=%1 candidate='%2' action=request-spawn-failed loadout=%3", playerId, spawnName, loadoutResource), LogLevel.WARNING);
				continue;
			}

			s_PendingSpawnRetries.RemoveItem(playerId);
			TrackHistoryStatic(playerId, spawnHistoryKey, config.AvoidLastNPerPlayer);
			CloseSpawnDialogStatic(playerId);
			Print(string.Format("[ARGH_SPAWN] player=%1 reserved='%2' action=request-spawn candidates=%3 filtered=%4 fallback=none", playerId, spawnName, fullPoolCount, filteredCandidates.Count()));
			return true;
		}

		ScheduleSpawnRetryStatic(playerId, "all-coastal-spawns-rejected");
		FallbackToCustomDialogStatic(playerId, "Coastal spawns were busy", "All valid coastal spawns were rejected or busy. Retrying automatically.");
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override protected void DoSpawn_S(int playerId)
	{
		Print(string.Format("[ARGH_TRACE] ARGH_RandomSpawnLogic.DoSpawn_S player=%1", playerId), LogLevel.WARNING);
		PrepareSpawnReservation_S(playerId);
	}

	//------------------------------------------------------------------------------------------------
	void RetrySpawnReservation_S(int playerId)
	{
		PrepareSpawnReservation_S(playerId);
	}

	//------------------------------------------------------------------------------------------------
	void ConfirmReservedSpawn_S(int playerId)
	{
		Print(string.Format("[ARGH_SPAWN] player=%1 action=confirm-respawn", playerId));
		PrepareSpawnReservation_S(playerId);
	}

	//------------------------------------------------------------------------------------------------
	protected void PrepareSpawnReservation_S(int playerId)
	{
		if (!TryStartCustomSpawnFlow_S(playerId))
		{
			FallbackToCustomDialog(playerId, "Spawn system unavailable", "Random coastal spawning is disabled or unavailable. Retry your spawn.");
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void CollectCandidates(int playerId, ARGH_RandomSpawnConfig config, out array<SCR_SpawnPoint> outCandidates)
	{
		CollectCandidatesStatic(playerId, config, outCandidates, true);
	}

	//------------------------------------------------------------------------------------------------
	protected bool IsAllowedSpawnName(ARGH_RandomSpawnConfig config, string spawnName)
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
	protected void FilterByRecentHistory(int playerId, array<SCR_SpawnPoint> source, out array<SCR_SpawnPoint> filtered, int avoidLastN)
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

		array<string> recentNames = GetRecentHistory(playerId);
		foreach (SCR_SpawnPoint spawnPoint : source)
		{
			if (!spawnPoint)
				continue;

			string historyKey = GetSpawnHistoryKeyStatic(spawnPoint);
			if (recentNames.Find(historyKey) == -1)
				filtered.Insert(spawnPoint);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected array<string> GetRecentHistory(int playerId)
	{
		return GetRecentHistoryStatic(playerId);
	}

	//------------------------------------------------------------------------------------------------
	protected void TrackHistory(int playerId, string spawnHistoryKey, int keepCount)
	{
		TrackHistoryStatic(playerId, spawnHistoryKey, keepCount);
	}

	//------------------------------------------------------------------------------------------------
	protected void FallbackToMenuOrWait(int playerId, string reason)
	{
		ARGH_RandomSpawnConfig config = GetConfig();
		m_sLastFallbackReason = reason;

		if (!config || config.FallbackToMenuOnEmptyPool)
		{
			SCR_RespawnComponent respawnComponent = GetPlayerRespawnComponent_S(playerId);
			if (respawnComponent)
				respawnComponent.NotifyReadyForSpawn_S();
			Print(string.Format("[ARGH_SPAWN] player=%1 fallback=menu reason='%2'", playerId, reason), LogLevel.WARNING);
			return;
		}

		Print(string.Format("[ARGH_SPAWN] player=%1 fallback=wait reason='%2'", playerId, reason), LogLevel.WARNING);
	}

	//------------------------------------------------------------------------------------------------
	protected void ShowSpawnReadyDialog(int playerId)
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!playerController)
			return;

		playerController.ARGH_ShowSpawnDialog_S("Coastal deployment ready", "Press Spawn to deploy into a random survivor coastal start.", false);
	}

	//------------------------------------------------------------------------------------------------
	protected void FallbackToCustomDialog(int playerId, string reason, string hint)
	{
		m_sLastFallbackReason = reason;

		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (playerController)
			playerController.ARGH_ShowSpawnDialog_S(reason, hint, true);

		Print(string.Format("[ARGH_SPAWN] player=%1 fallback=custom-dialog reason='%2'", playerId, reason), LogLevel.WARNING);
	}

	//------------------------------------------------------------------------------------------------
	protected void ApplyConfiguredFaction(int playerId, string configuredFactionKey)
	{
		FactionKey previousForcedFaction = m_sForcedFaction;
		if (!configuredFactionKey.IsEmpty())
			m_sForcedFaction = configuredFactionKey;

		Faction forcedFaction;
		if (GetForcedFaction(forcedFaction))
		{
			SCR_PlayerFactionAffiliationComponent factionComponent = GetPlayerFactionComponent_S(playerId);
			if (factionComponent)
				factionComponent.RequestFaction(forcedFaction);
		}

		m_sForcedFaction = previousForcedFaction;
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
		{
			Print(string.Format("[ARGH_SPAWN] Config missing, using defaults: %1", ARGH_RANDOM_SPAWN_CONFIG), LogLevel.WARNING);
			return s_Config;
		}

		BaseResourceObject resourceObject = configResource.GetResource();
		if (!resourceObject)
		{
			Print(string.Format("[ARGH_SPAWN] Config has no resource object, using defaults: %1", ARGH_RANDOM_SPAWN_CONFIG), LogLevel.WARNING);
			return s_Config;
		}

		BaseContainer container = resourceObject.ToBaseContainer();
		if (!container)
		{
			Print(string.Format("[ARGH_SPAWN] Config has no container, using defaults: %1", ARGH_RANDOM_SPAWN_CONFIG), LogLevel.WARNING);
			return s_Config;
		}

		Managed managedConfig = BaseContainerTools.CreateInstanceFromContainer(container);
		ARGH_RandomSpawnConfig loadedConfig = ARGH_RandomSpawnConfig.Cast(managedConfig);
		if (!loadedConfig)
		{
			Print(string.Format("[ARGH_SPAWN] Config parse failed, using defaults: %1", ARGH_RANDOM_SPAWN_CONFIG), LogLevel.WARNING);
			return s_Config;
		}

		if (!loadedConfig.AllowedSpawnNames)
			loadedConfig.AllowedSpawnNames = new array<string>();

		s_Config = loadedConfig;
		return s_Config;
	}

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

	//------------------------------------------------------------------------------------------------
	protected static bool HasConfiguredSpawnPoints(ARGH_RandomSpawnConfig config)
	{
		array<SCR_SpawnPoint> candidates = {};
		CollectCandidatesStatic(-1, config, candidates, false);
		return !candidates.IsEmpty();
	}

	//------------------------------------------------------------------------------------------------
	protected static bool IsMercOutScenario()
	{
		ChimeraGame game = GetGame();
		if (!game)
			return false;

		SCR_MissionHeader missionHeader = SCR_MissionHeader.Cast(game.GetMissionHeader());
		if (missionHeader)
		{
			if (missionHeader.m_sName == "ARGH SURVIVORS")
				return true;

			string missionWorldPath = missionHeader.GetWorldPath();
			if (missionWorldPath.Contains("MO_Kolguyev_ARGH.ent"))
				return true;
		}

		string activeWorldFile = game.GetWorldFile();
		return activeWorldFile.Contains("MO_Kolguyev_ARGH.ent");
	}

	//------------------------------------------------------------------------------------------------
	protected static void CollectCandidatesStatic(int playerId, ARGH_RandomSpawnConfig config, out array<SCR_SpawnPoint> outCandidates, bool requireReservation)
	{
		int spawnCount = SCR_SpawnPoint.CountSpawnPoints();
		for (int i = 0; i < spawnCount; i++)
		{
			SCR_SpawnPoint spawnPoint = SCR_SpawnPoint.GetSpawnPointByIndex(i);
			if (!spawnPoint)
				continue;

			if (!spawnPoint.IsSpawnPointEnabled() || !spawnPoint.IsSpawnPointActive())
				continue;

			string spawnName = spawnPoint.GetSpawnPointName();
			if (!IsAllowedSpawnNameStatic(config, spawnName))
				continue;

			if (requireReservation)
			{
				SCR_ESpawnResult reserveResult = SCR_ESpawnResult.SPAWN_NOT_ALLOWED;
				if (!spawnPoint.CanReserveFor_S(playerId, reserveResult))
					continue;
			}

			outCandidates.Insert(spawnPoint);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected static bool IsAllowedSpawnNameStatic(ARGH_RandomSpawnConfig config, string spawnName)
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
	protected static void FilterByRecentHistoryStatic(int playerId, array<SCR_SpawnPoint> source, out array<SCR_SpawnPoint> filtered, int avoidLastN)
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

		array<string> recentNames = GetRecentHistoryStatic(playerId);
		foreach (SCR_SpawnPoint spawnPoint : source)
		{
			if (!spawnPoint)
				continue;

			string historyKey = GetSpawnHistoryKeyStatic(spawnPoint);
			if (recentNames.Find(historyKey) == -1)
				filtered.Insert(spawnPoint);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected static array<string> GetRecentHistoryStatic(int playerId)
	{
		array<string> history;
		if (!s_PlayerRecentSpawns.Find(playerId, history))
			return new array<string>();

		return history;
	}

	//------------------------------------------------------------------------------------------------
	protected static void TrackHistoryStatic(int playerId, string spawnHistoryKey, int keepCount)
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
	protected static string GetSpawnHistoryKeyStatic(SCR_SpawnPoint spawnPoint)
	{
		if (!spawnPoint)
			return string.Empty;

		return string.Format("%1|%2", spawnPoint.GetSpawnPointName(), spawnPoint.GetRplId());
	}

	//------------------------------------------------------------------------------------------------
	protected static SCR_RespawnComponent GetPlayerRespawnComponentStatic(int playerId)
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!playerController)
			return null;

		return SCR_RespawnComponent.Cast(playerController.FindComponent(SCR_RespawnComponent));
	}

	//------------------------------------------------------------------------------------------------
	protected static SCR_PlayerFactionAffiliationComponent GetPlayerFactionComponentStatic(int playerId)
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!playerController)
			return null;

		return SCR_PlayerFactionAffiliationComponent.Cast(playerController.FindComponent(SCR_PlayerFactionAffiliationComponent));
	}

	//------------------------------------------------------------------------------------------------
	protected static SCR_PlayerLoadoutComponent GetPlayerLoadoutComponentStatic(int playerId)
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!playerController)
			return null;

		return SCR_PlayerLoadoutComponent.Cast(playerController.FindComponent(SCR_PlayerLoadoutComponent));
	}

	//------------------------------------------------------------------------------------------------
	protected static bool TryResolveSpawnLoadoutResourceStatic(int playerId, string configuredFactionKey, out ResourceName loadoutResource, out string failureReason)
	{
		loadoutResource = string.Empty;
		failureReason = string.Empty;

		SCR_PlayerFactionAffiliationComponent factionComponent = GetPlayerFactionComponentStatic(playerId);
		SCR_PlayerLoadoutComponent loadoutComponent = GetPlayerLoadoutComponentStatic(playerId);
		if (!factionComponent || !loadoutComponent)
		{
			failureReason = "player-components-pending";
			return false;
		}

		Faction faction = SCR_Faction.Cast(factionComponent.GetAffiliatedFaction());
		if (!faction)
			faction = ResolveSpawnFactionStatic(configuredFactionKey, playerId);

		if (!faction)
		{
			failureReason = "faction-unavailable";
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

			loadout = loadoutManager.GetPlayerLoadout(playerId);
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
	protected static Faction ResolveSpawnFactionStatic(string configuredFactionKey, int playerId)
	{
		Faction faction = ResolveConfiguredFactionStatic(configuredFactionKey);
		if (faction)
			return faction;

		SCR_PlayerFactionAffiliationComponent factionComponent = GetPlayerFactionComponentStatic(playerId);
		if (factionComponent)
		{
			Faction affiliatedFaction = factionComponent.GetAffiliatedFaction();
			if (affiliatedFaction)
			{
				Print(string.Format("[ARGH_SPAWN] action=faction-fallback configured='%1' resolved='%2'", configuredFactionKey, affiliatedFaction.GetFactionKey()), LogLevel.WARNING);
				return affiliatedFaction;
			}
		}

		Faction runtimeFallback = ResolveConfiguredFactionStatic(ARGH_RUNTIME_FALLBACK_FACTION);
		if (runtimeFallback)
		{
			Print(string.Format("[ARGH_SPAWN] action=faction-fallback configured='%1' resolved='%2'", configuredFactionKey, ARGH_RUNTIME_FALLBACK_FACTION), LogLevel.WARNING);
			return runtimeFallback;
		}

		SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
			return null;

		array<Faction> factions = {};
		factionManager.GetFactionsList(factions);
		foreach (Faction candidate : factions)
		{
			SCR_Faction scrFaction = SCR_Faction.Cast(candidate);
			if (scrFaction && scrFaction.IsPlayable())
			{
				Print(string.Format("[ARGH_SPAWN] action=faction-fallback configured='%1' resolved='%2'", configuredFactionKey, candidate.GetFactionKey()), LogLevel.WARNING);
				return candidate;
			}
		}

		return null;
	}

	//------------------------------------------------------------------------------------------------
	protected static Faction ResolveConfiguredFactionStatic(string configuredFactionKey)
	{
		if (configuredFactionKey.IsEmpty())
			return null;

		FactionKey resolvedFactionKey = configuredFactionKey;
		SCR_FactionAliasComponent factionAliasComponent = SCR_FactionAliasComponent.Cast(GetGame().GetFactionManager().FindComponent(SCR_FactionAliasComponent));
		if (factionAliasComponent)
			resolvedFactionKey = factionAliasComponent.ResolveFactionAlias(resolvedFactionKey);

		return GetGame().GetFactionManager().GetFactionByKey(resolvedFactionKey);
	}

	//------------------------------------------------------------------------------------------------
	protected static void ScheduleSpawnRetryStatic(int playerId, string reason)
	{
		if (s_PendingSpawnRetries.Contains(playerId))
			return;

		ScriptCallQueue callQueue = GetGame().GetCallqueue();
		if (!callQueue)
		{
			Print(string.Format("[ARGH_SPAWN] player=%1 retry-missed reason='%2' detail=no-callqueue", playerId, reason), LogLevel.WARNING);
			return;
		}

		s_PendingSpawnRetries.Insert(playerId);
		Print(string.Format("[ARGH_SPAWN] player=%1 action=retry-wait reason='%2'", playerId, reason), LogLevel.WARNING);
		callQueue.CallLater(ExecuteSpawnRetryStatic, 400, false, playerId);
	}

	//------------------------------------------------------------------------------------------------
	protected static void ExecuteSpawnRetryStatic(int playerId)
	{
		s_PendingSpawnRetries.RemoveItem(playerId);
		TryStartCustomSpawnFlow_S(playerId);
	}

	//------------------------------------------------------------------------------------------------
	protected static void ShowSpawnReadyDialogStatic(int playerId)
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!playerController)
			return;

		playerController.ARGH_ShowSpawnDialog_S("Coastal deployment ready", "Press Spawn to deploy into a random survivor coastal start.", false);
	}

	//------------------------------------------------------------------------------------------------
	protected static void CloseSpawnDialogStatic(int playerId)
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!playerController)
			return;

		playerController.ARGH_CloseSpawnDialog_S();
	}

	//------------------------------------------------------------------------------------------------
	protected static void FallbackToCustomDialogStatic(int playerId, string reason, string hint)
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (playerController)
			playerController.ARGH_ShowSpawnDialog_S(reason, hint, true);

		Print(string.Format("[ARGH_SPAWN] player=%1 fallback=custom-dialog reason='%2'", playerId, reason), LogLevel.WARNING);
	}

	//------------------------------------------------------------------------------------------------
	protected static void ApplyConfiguredFactionStatic(int playerId, string configuredFactionKey)
	{
		Faction forcedFaction = ResolveConfiguredFactionStatic(configuredFactionKey);
		if (!forcedFaction)
		{
			Print(string.Format("[ARGH_SPAWN] Missing faction '%1' for player=%2", configuredFactionKey, playerId), LogLevel.WARNING);
			return;
		}

		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!playerController)
			return;

		SCR_PlayerFactionAffiliationComponent factionComponent = SCR_PlayerFactionAffiliationComponent.Cast(playerController.FindComponent(SCR_PlayerFactionAffiliationComponent));
		if (!factionComponent)
			return;

		factionComponent.RequestFaction(forcedFaction);
	}
}

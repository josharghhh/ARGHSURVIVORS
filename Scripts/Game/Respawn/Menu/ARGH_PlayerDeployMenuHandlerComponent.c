modded class SCR_PlayerDeployMenuHandlerComponent
{
	protected static const float ARGH_DEATH_RESPAWN_COUNTDOWN = 20.0;
	protected static const int ARGH_DEATH_RESPAWN_COUNTDOWN_MS = 20000;
	protected static const float ARGH_CLASSIFICATION_TIMEOUT_SECONDS = 8.0;

	protected bool m_bARGH_NoDeployEnforced;
	protected bool m_bARGH_CountdownActive;
	protected float m_fARGH_CountdownRemaining;
	protected int m_iARGH_LastCountdownSecond = -1;
	protected bool m_bARGH_CountdownTimerArmed;
	protected int m_iARGH_CountdownSecondsRemaining;
	protected string m_sARGH_CountdownHint;
	protected string m_sARGH_LastCountdownHint;
	protected bool m_bARGH_AwaitingClassification;
	protected float m_fARGH_AwaitingClassificationElapsed;
	protected int m_iARGH_AwaitingClassificationLastSecond = -1;
	protected bool m_bARGH_InitialJoinWelcomeShown;
	protected bool m_bARGH_AwaitingClassificationMonitorScheduled;

	//------------------------------------------------------------------------------------------------
	protected bool ARGH_UsesHiddenAutoDeploy()
	{
		return ARGH_HiddenRandomSpawnService.IsActive();
	}

	//------------------------------------------------------------------------------------------------
	protected bool ARGH_ShouldHandleHiddenAutoDeploy()
	{
		if (!ARGH_UsesHiddenAutoDeploy())
			return false;

		SCR_PlayerController playerController = ARGH_GetLocalPlayerController();
		if (!playerController)
			return false;

		ARGH_RespawnMode respawnMode = playerController.ARGH_GetRespawnMode_O();
		if (respawnMode == ARGH_RespawnMode.InitialJoin)
			return false;

		return respawnMode != ARGH_RespawnMode.None;
	}

	//------------------------------------------------------------------------------------------------
	protected SCR_PlayerController ARGH_GetLocalPlayerController()
	{
		return SCR_PlayerController.Cast(GetGame().GetPlayerController());
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_DestroyLoadingPlaceholder()
	{
		SCR_RespawnSystemComponent respawnSystem = SCR_RespawnSystemComponent.GetInstance();
		if (respawnSystem)
			respawnSystem.DestroyLoadingPlaceholder();
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_CloseStockDeployMenus()
	{
		MenuManager menuManager = GetGame().GetMenuManager();
		if (menuManager)
		{
			menuManager.CloseMenuByPreset(ChimeraMenuPreset.WelcomeScreenMenu);
			menuManager.CloseMenuByPreset(ChimeraMenuPreset.RespawnSuperMenu);
			menuManager.CloseMenuByPreset(ChimeraMenuPreset.RoleSelectionDialog);
		}

		SCR_DeployMenuMain.CloseDeployMenu();
		SCR_RoleSelectionMenu.CloseRoleSelectionMenu();
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_RequestHiddenSpawn()
	{
		if (!ARGH_UsesHiddenAutoDeploy())
			return;

		Print("[ARGH_AUTODEPLOY] action=deploy-handler-request", LogLevel.WARNING);
		ARGH_EnforceNoDeployUi();
		ARGH_StopDeathCountdown(false);

		SCR_PlayerController playerController = ARGH_GetLocalPlayerController();
		if (playerController)
			playerController.ARGH_RequestForcedRandomRespawn();

		ARGH_HiddenRandomSpawnService.TryRequestLocalSpawn();
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_ShowDeathCountdown()
	{
		int secondsRemaining = Math.Ceil(m_fARGH_CountdownRemaining);
		if (secondsRemaining < 1)
			secondsRemaining = 1;

		if (m_iARGH_LastCountdownSecond == secondsRemaining)
			return;

		m_iARGH_LastCountdownSecond = secondsRemaining;
		string status = string.Format("Respawning in %1", secondsRemaining);
		string hint = m_sARGH_CountdownHint;
		if (hint.IsEmpty())
		{
			hint = ARGH_SelectDeathCountdownHint();
			m_sARGH_CountdownHint = hint;
		}
		ARGH_SpawnDialog.UpdateCurrent(status, hint, false, true);
		ARGH_SpawnDialog.SetCurrentStatusColor(new Color(1.0, 0.2, 0.2, 1.0));
	}

	//------------------------------------------------------------------------------------------------
	protected string ARGH_SelectDeathCountdownHint()
	{
		SCR_PlayerController playerController = ARGH_GetLocalPlayerController();
		ARGH_DeathReason reason = ARGH_DeathReason.Death;
		if (playerController)
			reason = playerController.ARGH_GetDeathReason_O();

		if (reason == ARGH_DeathReason.Suicide)
		{
			array<string> suicideHints = {
				"Bold strategy. New body arrives in a few seconds.",
				"Rage quit denied. Coastal redeploy is loading.",
				"Self-checkout complete. Please wait for replacement.",
				"You pressed the panic button. Inserting you elsewhere."
			};

			return ARGH_SelectHintNoRepeat(suicideHints);
		}

		array<string> deathHints = {
			"You got folded. Respawn prep underway.",
			"That was not survivable. Reprinting survivor now.",
			"Medic called it. Coastal insertion queued.",
			"Death noted. Sending you somewhere less terrible."
		};

		return ARGH_SelectHintNoRepeat(deathHints);
	}

	//------------------------------------------------------------------------------------------------
	protected string ARGH_SelectHintNoRepeat(notnull array<string> hints)
	{
		if (hints.IsEmpty())
			return "Respawning soon.";

		if (hints.Count() == 1)
		{
			m_sARGH_LastCountdownHint = hints[0];
			return hints[0];
		}

		int startIndex = Math.RandomInt(0, hints.Count());
		for (int i = 0; i < hints.Count(); i++)
		{
			int idx = (startIndex + i) % hints.Count();
			string candidate = hints[idx];
			if (candidate == m_sARGH_LastCountdownHint)
				continue;

			m_sARGH_LastCountdownHint = candidate;
			return candidate;
		}

		// Fallback should never happen, but keep behavior safe.
		m_sARGH_LastCountdownHint = hints[0];
		return hints[0];
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_StartDeathCountdown()
	{
		if (m_bARGH_CountdownActive)
			return;

		m_bARGH_CountdownActive = true;
		m_fARGH_CountdownRemaining = ARGH_DEATH_RESPAWN_COUNTDOWN;
		m_iARGH_LastCountdownSecond = -1;
		m_iARGH_CountdownSecondsRemaining = ARGH_DEATH_RESPAWN_COUNTDOWN;
		m_bARGH_CountdownTimerArmed = true;
		m_sARGH_CountdownHint = ARGH_SelectDeathCountdownHint();
		GetGame().GetCallqueue().CallLater(ARGH_OnDeathCountdownElapsed, ARGH_DEATH_RESPAWN_COUNTDOWN_MS, false);
		GetGame().GetCallqueue().CallLater(ARGH_OnDeathCountdownTick, 1000, true);
		Print(string.Format("[ARGH_PERSIST] action=death-countdown-start seconds=%1", ARGH_DEATH_RESPAWN_COUNTDOWN), LogLevel.WARNING);
		ARGH_SpawnDialog.OpenPassiveDialog("Respawning in 20", m_sARGH_CountdownHint);
		ARGH_ShowDeathCountdown();
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_OnDeathCountdownTick()
	{
		if (!m_bARGH_CountdownActive)
			return;

		SCR_PlayerController playerController = ARGH_GetLocalPlayerController();
		if (!playerController || ARGH_HiddenRandomSpawnService.HasAliveControlledEntity(playerController))
		{
			ARGH_StopDeathCountdown();
			return;
		}

		m_iARGH_CountdownSecondsRemaining -= 1;
		if (m_iARGH_CountdownSecondsRemaining < 0)
			m_iARGH_CountdownSecondsRemaining = 0;

		m_fARGH_CountdownRemaining = m_iARGH_CountdownSecondsRemaining;
		ARGH_ShowDeathCountdown();

		if (m_iARGH_CountdownSecondsRemaining == 0)
			ARGH_OnDeathCountdownElapsed();
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_OnDeathCountdownElapsed()
	{
		if (!m_bARGH_CountdownActive || !m_bARGH_CountdownTimerArmed)
			return;

		m_bARGH_CountdownTimerArmed = false;

		SCR_PlayerController playerController = ARGH_GetLocalPlayerController();
		if (!playerController || ARGH_HiddenRandomSpawnService.HasAliveControlledEntity(playerController))
		{
			ARGH_StopDeathCountdown();
			return;
		}

		ARGH_StopDeathCountdown(false);
		ARGH_SpawnDialog.OpenPassiveDialog("Respawning survivor.", "Preparing your random coastal fallback spawn.");
		ARGH_SpawnDialog.SetCurrentStatusColor(Color.White);
		Print("[ARGH_PERSIST] action=death-countdown-complete", LogLevel.WARNING);
		ARGH_RequestHiddenSpawn();
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_StopDeathCountdown(bool closeDialog = true)
	{
		m_bARGH_CountdownActive = false;
		m_fARGH_CountdownRemaining = 0;
		m_iARGH_LastCountdownSecond = -1;
		m_bARGH_CountdownTimerArmed = false;
		m_iARGH_CountdownSecondsRemaining = 0;
		m_sARGH_CountdownHint = string.Empty;
		GetGame().GetCallqueue().Remove(ARGH_OnDeathCountdownElapsed);
		GetGame().GetCallqueue().Remove(ARGH_OnDeathCountdownTick);

		if (closeDialog)
			ARGH_SpawnDialog.CloseCurrent();
		else
			ARGH_SpawnDialog.SetCurrentStatusColor(Color.White);
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_UpdateDeathCountdown(float timeSlice)
	{
		if (!m_bARGH_CountdownActive)
			return;

		SCR_PlayerController playerController = ARGH_GetLocalPlayerController();
		if (!playerController || ARGH_HiddenRandomSpawnService.HasAliveControlledEntity(playerController))
		{
			ARGH_StopDeathCountdown();
			return;
		}

		m_fARGH_CountdownRemaining -= timeSlice;
		if (m_fARGH_CountdownRemaining > 0)
		{
			ARGH_ShowDeathCountdown();
			return;
		}

		ARGH_StopDeathCountdown(false);
		ARGH_SpawnDialog.OpenPassiveDialog("Respawning survivor.", "Preparing your random coastal fallback spawn.");
		ARGH_SpawnDialog.SetCurrentStatusColor(Color.White);
		Print("[ARGH_PERSIST] action=death-countdown-complete", LogLevel.WARNING);
		ARGH_RequestHiddenSpawn();
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_ResetClassificationTimeout()
	{
		ARGH_StopAwaitingClassificationMonitor();
		m_bARGH_AwaitingClassification = false;
		m_fARGH_AwaitingClassificationElapsed = 0;
		m_iARGH_AwaitingClassificationLastSecond = -1;
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_StartAwaitingClassificationMonitor()
	{
		if (m_bARGH_AwaitingClassificationMonitorScheduled)
			return;

		ScriptCallQueue callQueue = GetGame().GetCallqueue();
		if (!callQueue)
			return;

		m_bARGH_AwaitingClassificationMonitorScheduled = true;
		callQueue.CallLater(ARGH_OnAwaitingClassificationMonitorElapsed, ARGH_CLASSIFICATION_TIMEOUT_SECONDS * 1000, false);
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_StopAwaitingClassificationMonitor()
	{
		if (!m_bARGH_AwaitingClassificationMonitorScheduled)
			return;

		ScriptCallQueue callQueue = GetGame().GetCallqueue();
		if (callQueue)
			callQueue.Remove(ARGH_OnAwaitingClassificationMonitorElapsed);

		m_bARGH_AwaitingClassificationMonitorScheduled = false;
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_OnAwaitingClassificationMonitorElapsed()
	{
		m_bARGH_AwaitingClassificationMonitorScheduled = false;

		SCR_PlayerController playerController = ARGH_GetLocalPlayerController();
		if (!playerController)
			return;

		if (playerController.ARGH_IsRestorePending_O() || playerController.ARGH_GetRespawnMode_O() != ARGH_RespawnMode.None)
			return;

		Print(string.Format("[ARGH_PERSIST] action=awaiting-classification-timeout-fallback timeout=%1 source='watchdog'", ARGH_CLASSIFICATION_TIMEOUT_SECONDS), LogLevel.WARNING);
		playerController.ARGH_RequestForcedRandomRespawn();
		Print("[ARGH_PERSIST] action=awaiting-classification-forced mode='ForcedRandomRespawn'", LogLevel.WARNING);
		ARGH_ResetClassificationTimeout();
		ARGH_EnforceNoDeployUi();
		ARGH_RequestHiddenSpawn();
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_UpdateClassificationTimeout(float timeSlice)
	{
		SCR_PlayerController playerController = ARGH_GetLocalPlayerController();
		if (!playerController)
		{
			ARGH_ResetClassificationTimeout();
			return;
		}

		if (playerController.ARGH_IsRestorePending_O() || playerController.ARGH_GetRespawnMode_O() != ARGH_RespawnMode.None)
		{
			ARGH_ResetClassificationTimeout();
			return;
		}

		if (!m_bARGH_AwaitingClassification)
		{
			m_bARGH_AwaitingClassification = true;
			m_fARGH_AwaitingClassificationElapsed = 0;
			m_iARGH_AwaitingClassificationLastSecond = -1;
		}

		m_fARGH_AwaitingClassificationElapsed += timeSlice;
		int wholeSeconds = Math.Floor(m_fARGH_AwaitingClassificationElapsed);
		if (wholeSeconds != m_iARGH_AwaitingClassificationLastSecond)
		{
			m_iARGH_AwaitingClassificationLastSecond = wholeSeconds;
			Print(string.Format("[ARGH_PERSIST] action=awaiting-classification-elapsed seconds=%1", wholeSeconds), LogLevel.WARNING);
		}

		if (m_fARGH_AwaitingClassificationElapsed < ARGH_CLASSIFICATION_TIMEOUT_SECONDS)
			return;

		Print(string.Format("[ARGH_PERSIST] action=awaiting-classification-timeout-fallback timeout=%1", ARGH_CLASSIFICATION_TIMEOUT_SECONDS), LogLevel.WARNING);
		playerController.ARGH_RequestForcedRandomRespawn();
		Print("[ARGH_PERSIST] action=awaiting-classification-forced mode='ForcedRandomRespawn'", LogLevel.WARNING);
		ARGH_ResetClassificationTimeout();
		ARGH_EnforceNoDeployUi();
		ARGH_RequestHiddenSpawn();
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_StopNoDeployEnforcement()
	{
		if (!m_bARGH_NoDeployEnforced)
			return;

		ScriptCallQueue callQueue = GetGame().GetCallqueue();
		if (callQueue)
			callQueue.Remove(ARGH_EnforceNoDeployUi);

		m_bARGH_NoDeployEnforced = false;
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_StartNoDeployEnforcement()
	{
		if (m_bARGH_NoDeployEnforced)
			return;

		ScriptCallQueue callQueue = GetGame().GetCallqueue();
		if (callQueue)
			callQueue.CallLater(ARGH_EnforceNoDeployUi, 50, true);

		m_bARGH_NoDeployEnforced = true;
		ARGH_EnforceNoDeployUi();
	}

	//------------------------------------------------------------------------------------------------
	protected void ARGH_EnforceNoDeployUi()
	{
		if (!ARGH_UsesHiddenAutoDeploy())
		{
			ARGH_StopDeathCountdown();
			ARGH_StopNoDeployEnforcement();
			return;
		}

		SetWelcomeClosed();
		ARGH_CloseStockDeployMenus();

		SCR_RespawnSystemComponent respawnSystem = SCR_RespawnSystemComponent.GetInstance();
		if (respawnSystem)
			respawnSystem.DestroyLoadingPlaceholder();
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);

		if (!GetGame().InPlayMode())
			return;
	}

	//------------------------------------------------------------------------------------------------
	override void Update(float timeSlice)
	{
		SCR_PlayerController playerController = ARGH_GetLocalPlayerController();
		if (!playerController || playerController.ARGH_GetRespawnMode_O() != ARGH_RespawnMode.InitialJoin)
			m_bARGH_InitialJoinWelcomeShown = false;
		else if (!m_bARGH_InitialJoinWelcomeShown && !playerController.ARGH_IsRestorePending_O() && !ARGH_HiddenRandomSpawnService.HasAliveControlledEntity(playerController))
		{
			Print("[ARGH_PERSIST] action=initial-join-dialog-open", LogLevel.WARNING);
			MenuManager menuManager = GetGame().GetMenuManager();
			if (menuManager)
				menuManager.CloseMenuByPreset(ChimeraMenuPreset.WelcomeScreenMenu);

			ARGH_DestroyLoadingPlaceholder();
			ARGH_SpawnDialog dialog = ARGH_SpawnDialog.OpenDialog(
				"WELCOME TO ARGH SURVIVORS",
				"Press spawn to deploy at a random coastal survivor start.",
				false
			);

			if (dialog)
			{
				m_bARGH_InitialJoinWelcomeShown = true;
			}
			else
			{
				Print("[ARGH_PERSIST] action=initial-join-dialog-open-failed fallback='forced-random-respawn'", LogLevel.ERROR);
				playerController.ARGH_RequestForcedRandomRespawn();
				ARGH_RequestHiddenSpawn();
			}
		}

		if (ARGH_ShouldHandleHiddenAutoDeploy())
		{
			ARGH_EnforceNoDeployUi();
			ARGH_UpdateDeathCountdown(timeSlice);
			ARGH_ResetClassificationTimeout();

			if (playerController && ARGH_HiddenRandomSpawnService.HasAliveControlledEntity(playerController) && !m_bARGH_CountdownActive)
				ARGH_SpawnDialog.CloseCurrent();

			return;
		}

		ARGH_StopDeathCountdown();
		ARGH_StopNoDeployEnforcement();
		ARGH_UpdateClassificationTimeout(timeSlice);
		super.Update(timeSlice);
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnRespawnReady()
	{
		if (ARGH_UsesHiddenAutoDeploy())
		{
			SCR_PlayerController playerController = ARGH_GetLocalPlayerController();
			if (!playerController)
				return;

			if (playerController.ARGH_IsRestorePending_O())
			{
				Print("[ARGH_PERSIST] action=respawn-ready-suppressed reason='restore-pending'", LogLevel.WARNING);
				return;
			}

			if (ARGH_HiddenRandomSpawnService.HasPendingLocalRequest())
				return;

			ARGH_RespawnMode respawnMode = playerController.ARGH_GetRespawnMode_O();
			if (respawnMode == ARGH_RespawnMode.None)
			{
				ARGH_StartNoDeployEnforcement();
				Print("[ARGH_PERSIST] action=respawn-ready-suppressed reason='awaiting-classification'", LogLevel.WARNING);
				ARGH_UpdateClassificationTimeout(0.0);
				ARGH_StartAwaitingClassificationMonitor();
				return;
			}

			ARGH_ResetClassificationTimeout();

			if (respawnMode == ARGH_RespawnMode.InitialJoin)
			{
				if (!m_bARGH_InitialJoinWelcomeShown)
				{
					Print("[ARGH_PERSIST] action=respawn-ready-initial-join-dialog", LogLevel.WARNING);
					MenuManager menuManager = GetGame().GetMenuManager();
					if (menuManager)
						menuManager.CloseMenuByPreset(ChimeraMenuPreset.WelcomeScreenMenu);

					ARGH_DestroyLoadingPlaceholder();
					ARGH_SpawnDialog dialog = ARGH_SpawnDialog.OpenDialog(
						"WELCOME TO ARGH SURVIVORS",
						"Press spawn to deploy at a random coastal survivor start.",
						false
					);

					if (dialog)
					{
						m_bARGH_InitialJoinWelcomeShown = true;
					}
					else
					{
						Print("[ARGH_PERSIST] action=respawn-ready-initial-join-dialog-failed fallback='forced-random-respawn'", LogLevel.ERROR);
						playerController.ARGH_RequestForcedRandomRespawn();
						ARGH_RequestHiddenSpawn();
					}
				}
				return;
			}

			if (respawnMode == ARGH_RespawnMode.ReconnectRestore)
			{
				ARGH_StartNoDeployEnforcement();
				Print("[ARGH_PERSIST] action=respawn-ready-suppressed reason='reconnect-restore'", LogLevel.WARNING);
				return;
			}

			if (respawnMode == ARGH_RespawnMode.DeathRespawn)
			{
				ARGH_StartNoDeployEnforcement();
				ARGH_StartDeathCountdown();
				return;
			}

			if (respawnMode == ARGH_RespawnMode.ForcedRandomRespawn)
			{
				ARGH_StartNoDeployEnforcement();
				Print("[ARGH_PERSIST] action=forced-random-respawn-requested", LogLevel.WARNING);
			}

			ScriptCallQueue callQueue = GetGame().GetCallqueue();
			if (callQueue)
				callQueue.Call(ARGH_RequestHiddenSpawn);
			else
				ARGH_RequestHiddenSpawn();
			return;
		}

		super.OnRespawnReady();
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnPlayerReconnect(int state)
	{
		if (ARGH_ShouldHandleHiddenAutoDeploy())
		{
			ARGH_EnforceNoDeployUi();
			return;
		}

		super.OnPlayerReconnect(state);
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnDelete(IEntity owner)
	{
		ARGH_StopDeathCountdown();
		ARGH_StopNoDeployEnforcement();
		ARGH_ResetClassificationTimeout();
		super.OnDelete(owner);
	}
}

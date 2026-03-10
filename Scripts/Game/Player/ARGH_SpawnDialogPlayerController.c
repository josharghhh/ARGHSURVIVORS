enum ARGH_RespawnMode
{
	None,
	ReconnectRestore,
	InitialJoin,
	DeathRespawn,
	RestoreFallback,
	ForcedRandomRespawn
}

enum ARGH_DeathReason
{
	Unknown,
	Death,
	Suicide
}

modded class SCR_PlayerController
{
	protected ARGH_RespawnMode m_eARGH_RespawnMode = ARGH_RespawnMode.None;
	protected bool m_bARGH_RestorePending;
	protected ARGH_DeathReason m_eARGH_DeathReason = ARGH_DeathReason.Unknown;

	//------------------------------------------------------------------------------------------------
	void ARGH_RequestSpawnDialogAction(bool retryMode)
	{
		if (retryMode)
			Rpc(ARGH_RpcRetrySpawnDialog);
		else
			Rpc(ARGH_RpcConfirmSpawnDialog);
	}

	//------------------------------------------------------------------------------------------------
	void ARGH_ShowSpawnDialog_S(string status, string hint, bool retryMode)
	{
		Rpc(ARGH_RpcShowSpawnDialog, status, hint, retryMode);
	}

	//------------------------------------------------------------------------------------------------
	void ARGH_CloseSpawnDialog_S()
	{
		Rpc(ARGH_RpcCloseSpawnDialog);
	}

	//------------------------------------------------------------------------------------------------
	void ARGH_SetRespawnMode_S(ARGH_RespawnMode mode)
	{
		m_eARGH_RespawnMode = mode;
		Rpc(ARGH_RpcSetRespawnMode_O, mode);
	}

	//------------------------------------------------------------------------------------------------
	ARGH_RespawnMode ARGH_GetRespawnMode_O()
	{
		return m_eARGH_RespawnMode;
	}

	//------------------------------------------------------------------------------------------------
	void ARGH_SetRestorePending_S(bool pending)
	{
		m_bARGH_RestorePending = pending;
		Rpc(ARGH_RpcSetRestorePending_O, pending);
	}

	//------------------------------------------------------------------------------------------------
	void ARGH_SetDeathReason_S(ARGH_DeathReason reason)
	{
		m_eARGH_DeathReason = reason;
		Rpc(ARGH_RpcSetDeathReason_O, reason);
	}

	//------------------------------------------------------------------------------------------------
	ARGH_DeathReason ARGH_GetDeathReason_O()
	{
		return m_eARGH_DeathReason;
	}

	//------------------------------------------------------------------------------------------------
	void ARGH_RequestManualSuicideRespawn()
	{
		m_eARGH_DeathReason = ARGH_DeathReason.Suicide;
		m_bARGH_RestorePending = false;
		m_eARGH_RespawnMode = ARGH_RespawnMode.DeathRespawn;
		Rpc(ARGH_RpcSetRestorePending_O, false);
		Rpc(ARGH_RpcSetRespawnMode_O, ARGH_RespawnMode.DeathRespawn);
		Rpc(ARGH_RpcRequestManualSuicideRespawn_S);
	}

	//------------------------------------------------------------------------------------------------
	void ARGH_RequestForcedRandomRespawn()
	{
		m_bARGH_RestorePending = false;
		m_eARGH_RespawnMode = ARGH_RespawnMode.ForcedRandomRespawn;
		Rpc(ARGH_RpcRequestForcedRandomRespawn_S);
	}

	//------------------------------------------------------------------------------------------------
	void ARGH_RequestPauseExitSave()
	{
		Rpc(ARGH_RpcRequestPauseExitSave_S);
	}

	//------------------------------------------------------------------------------------------------
	bool ARGH_CanRequestPauseExitSave_O()
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	bool ARGH_IsRestorePending_O()
	{
		return m_bARGH_RestorePending;
	}

	//------------------------------------------------------------------------------------------------
	override void OnControlledEntityChanged(IEntity from, IEntity to)
	{
		super.OnControlledEntityChanged(from, to);
		ARGH_HiddenRandomSpawnService.NotifyLocalControlledEntityChanged(this, to);

		if (!Replication.IsServer() && to)
		{
			ChimeraCharacter character = ChimeraCharacter.Cast(to);
			if (character)
			{
				CharacterControllerComponent characterController = character.GetCharacterController();
				if (characterController && !characterController.IsDead())
				{
					m_bARGH_RestorePending = false;
					m_eARGH_RespawnMode = ARGH_RespawnMode.None;
					m_eARGH_DeathReason = ARGH_DeathReason.Unknown;
				}
			}

			ARGH_SpawnDialog.CloseCurrent();
			MenuManager menuManager = GetGame().GetMenuManager();
			if (menuManager)
			{
				menuManager.CloseMenuByPreset(ChimeraMenuPreset.WelcomeScreenMenu);
				menuManager.CloseMenuByPreset(ChimeraMenuPreset.RespawnSuperMenu);
				menuManager.CloseMenuByPreset(ChimeraMenuPreset.RoleSelectionDialog);
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void ARGH_RpcConfirmSpawnDialog()
	{
		ARGH_RandomSpawnLogic spawnLogic = ARGH_RandomSpawnLogic.GetInstance();
		Print(string.Format("[ARGH_PERSIST] player=%1 action=spawn-dialog-confirm-rpc mode='%2' has-logic=%3", GetPlayerId(), typename.EnumToString(ARGH_RespawnMode, m_eARGH_RespawnMode), spawnLogic != null), LogLevel.WARNING);
		if (spawnLogic)
			spawnLogic.ConfirmReservedSpawn_S(GetPlayerId());
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void ARGH_RpcRetrySpawnDialog()
	{
		ARGH_RandomSpawnLogic spawnLogic = ARGH_RandomSpawnLogic.GetInstance();
		Print(string.Format("[ARGH_PERSIST] player=%1 action=spawn-dialog-retry-rpc mode='%2' has-logic=%3", GetPlayerId(), typename.EnumToString(ARGH_RespawnMode, m_eARGH_RespawnMode), spawnLogic != null), LogLevel.WARNING);
		if (spawnLogic)
			spawnLogic.RetrySpawnReservation_S(GetPlayerId());
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void ARGH_RpcRequestForcedRandomRespawn_S()
	{
		m_bARGH_RestorePending = false;
		m_eARGH_RespawnMode = ARGH_RespawnMode.ForcedRandomRespawn;
		Rpc(ARGH_RpcSetRestorePending_O, false);
		Rpc(ARGH_RpcSetRespawnMode_O, ARGH_RespawnMode.ForcedRandomRespawn);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void ARGH_RpcRequestManualSuicideRespawn_S()
	{
		m_eARGH_DeathReason = ARGH_DeathReason.Suicide;
		m_bARGH_RestorePending = false;
		m_eARGH_RespawnMode = ARGH_RespawnMode.DeathRespawn;
		Rpc(ARGH_RpcSetDeathReason_O, ARGH_DeathReason.Suicide);
		Rpc(ARGH_RpcSetRestorePending_O, false);
		Rpc(ARGH_RpcSetRespawnMode_O, ARGH_RespawnMode.DeathRespawn);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void ARGH_RpcRequestPauseExitSave_S()
	{
		SCR_RespawnSystemComponent respawnSystem = SCR_RespawnSystemComponent.GetInstance();
		if (!respawnSystem)
			return;

		SCR_SpawnLogic spawnLogic = respawnSystem.GetSpawnLogic();
		if (spawnLogic)
			spawnLogic.ARGH_RequestPauseExitSave_S(GetPlayerId());
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void ARGH_RpcShowSpawnDialog(string status, string hint, bool retryMode)
	{
		MenuManager menuManager = GetGame().GetMenuManager();
		if (menuManager)
		{
			menuManager.CloseMenuByPreset(ChimeraMenuPreset.WelcomeScreenMenu);
			menuManager.CloseMenuByPreset(ChimeraMenuPreset.RespawnSuperMenu);
			menuManager.CloseMenuByPreset(ChimeraMenuPreset.RoleSelectionDialog);
		}

		ARGH_SpawnDialog.OpenDialog(status, hint, retryMode);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void ARGH_RpcCloseSpawnDialog()
	{
		ARGH_SpawnDialog.CloseCurrent();
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void ARGH_RpcSetRespawnMode_O(ARGH_RespawnMode mode)
	{
		m_eARGH_RespawnMode = mode;
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void ARGH_RpcSetRestorePending_O(bool pending)
	{
		m_bARGH_RestorePending = pending;
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void ARGH_RpcSetDeathReason_O(ARGH_DeathReason reason)
	{
		m_eARGH_DeathReason = reason;
	}
}

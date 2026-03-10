modded class SCR_WelcomeScreenMenu
{
	override void OnMenuOpen()
	{
		if (ARGH_HiddenRandomSpawnService.IsActive())
		{
			SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
			if (!playerController || playerController.ARGH_GetRespawnMode_O() != ARGH_RespawnMode.InitialJoin)
			{
				Close();
				return;
			}
		}

		super.OnMenuOpen();
	}

	override void OnMenuClose()
	{
		super.OnMenuClose();

		if (!ARGH_HiddenRandomSpawnService.IsActive())
			return;

		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!playerController)
			return;

		if (playerController.ARGH_GetRespawnMode_O() != ARGH_RespawnMode.InitialJoin)
			return;

		if (playerController.ARGH_IsRestorePending_O())
			return;

		if (ARGH_HiddenRandomSpawnService.HasAliveControlledEntity(playerController))
			return;

		if (ARGH_HiddenRandomSpawnService.HasPendingLocalRequest())
			return;

		Print("[ARGH_PERSIST] action=initial-join-welcome-continue", LogLevel.WARNING);
		playerController.ARGH_RequestForcedRandomRespawn();
		ARGH_HiddenRandomSpawnService.TryRequestLocalSpawn();
	}
}

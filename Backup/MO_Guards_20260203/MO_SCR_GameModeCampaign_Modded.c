modded class SCR_GameModeCampaign
{
	override void HandleStartup()
	{
		const SaveGameManager manager = GetGame().GetSaveGameManager();
		manager.SetSavingAllowed(true);
		manager.RetrieveSaveGameInfo({manager.GetCurrentMissionResource()}, new SaveGameOperationCb(OnLoad));
		
		MO_GameModeStateSystem stateSystem = MO_GameModeStateSystem.GetInstance();
		if (!stateSystem)
			return;
		
		if (stateSystem.callQueue)
			stateSystem.callQueue.CallLater(stateSystem.InitScavSpawns, 1000, false);
		else
			GetGame().GetCallqueue().CallLater(stateSystem.InitScavSpawns, 1000, false);
	}
}

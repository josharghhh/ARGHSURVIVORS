class DZ_SaveGameUtil
{
	static bool OverwriteLatestSave(SaveGameManager saveManager, SaveGameOperationCb cb = null)
	{
		if (!saveManager)
			return false;

		if (!saveManager.IsSavingAllowed())
			return false;

		if (!saveManager.IsSavingPossible())
			return false;

		if (saveManager.IsBusy())
			return false;

		SaveGame active = saveManager.GetActiveSave();
		ESaveGameRequestFlags reqFlags = 0; // explicit non-blocking flags
		if (active)
		{
			bool ok = saveManager.RequestSavePointOverwrite(active, reqFlags, cb);
			return ok;
		}

		array<SaveGame> saves = {};
		int count = saveManager.GetSaves(saves);
		if (count > 0)
		{
			SaveGame latest = saves[count - 1];
			bool okLatest = saveManager.RequestSavePointOverwrite(latest, reqFlags, cb);
			return okLatest;
		}

		return false;
	}
}

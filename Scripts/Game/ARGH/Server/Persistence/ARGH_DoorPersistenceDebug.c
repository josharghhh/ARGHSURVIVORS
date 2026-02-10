class ARGH_DoorPersistenceDebug
{
	protected static const bool ENABLED = true;

	static bool IsEnabled()
	{
		return ENABLED;
	}

	static void Trace(string message, LogLevel level = LogLevel.WARNING)
	{
		if (!ENABLED)
			return;

		Print(string.Format("[ARGH-DOOR-DBG] %1", message), level);
	}

	static void TraceEnsureStart(IEntity ownerEnt, string ownerPrefab, string myDoor, string myCode, string doorChildPrefab, int attemptsLeft)
	{
		if (!ENABLED)
			return;

		string ownerName = "<null>";
		if (ownerEnt)
			ownerName = ownerEnt.ToString();

		Trace(string.Format("Ensure start owner=%1 prefab=%2 myDoor=%3 codeLen=%4 childPrefab=%5 attempts=%6",
			ownerName, ownerPrefab, myDoor, myCode.Length(), doorChildPrefab, attemptsLeft));
	}

	static void TraceSpawnResult(bool success, string prefab, string newDoorId, bool hasDoorComponent, bool hasOwnership)
	{
		if (!ENABLED)
			return;

		Trace(string.Format("Spawn result success=%1 prefab=%2 doorId=%3 hasDoorComp=%4 hasOwn=%5",
			success, prefab, newDoorId, hasDoorComponent, hasOwnership));
	}
}

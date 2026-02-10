class DZ_PersistenceCommit
{
	protected static bool s_bFlushQueued = false;
	protected static int s_iNextAllowedFlushMs = 0;

	protected static string s_sPendingReason = string.Empty;
	protected static ESaveGameType s_ePendingSaveType = ESaveGameType.MANUAL;

	protected static bool s_bFlushInFlight = false;
	protected static string s_sInFlightReason = string.Empty;
	protected static ESaveGameType s_eInFlightSaveType = ESaveGameType.MANUAL;

	protected static bool s_bRetryScheduled = false;
	protected static int s_iRetryCount = 0;

	protected static const int MAX_RETRIES = 10;
	protected static const int RETRY_DELAY_MS = 1000;

	protected static ref ScriptInvoker s_OnFlushCompleted;

	//------------------------------------------------------------------------------------------------
	static ScriptInvoker GetOnFlushCompleted()
	{
		if (!s_OnFlushCompleted)
			s_OnFlushCompleted = new ScriptInvoker();

		return s_OnFlushCompleted;
	}

	//------------------------------------------------------------------------------------------------
	static void RequestCommit_S(string reason, int minIntervalMs = 30000, int delayMs = 2000)
	{
		RequestGlobalFlush_S(reason, minIntervalMs, delayMs, ESaveGameType.MANUAL);
	}

	//------------------------------------------------------------------------------------------------
	static void RequestGlobalFlush_S(string reason, int minIntervalMs = 30000, int delayMs = 2000, ESaveGameType saveType = ESaveGameType.MANUAL)
	{
		if (!Replication.IsServer())
			return;

		s_sPendingReason = reason;
		s_ePendingSaveType = saveType;

		if (s_bFlushInFlight)
		{
			Print(string.Format("[DZ-SAVE] commit coalesced (in-flight) pendingReason=%1", reason), LogLevel.NORMAL);
			return;
		}

		if (s_bRetryScheduled)
		{
			Print(string.Format("[DZ-SAVE] commit coalesced (retry-loop) pendingReason=%1", reason), LogLevel.NORMAL);
			return;
		}

		int now = System.GetTickCount();

		if (now < s_iNextAllowedFlushMs)
		{
			Print(string.Format("[DZ-SAVE] commit skipped (throttle) reason=%1", reason), LogLevel.NORMAL);
			return;
		}

		if (s_bFlushQueued)
		{
			Print(string.Format("[DZ-SAVE] commit already queued reason=%1", reason), LogLevel.NORMAL);
			return;
		}

		s_bFlushQueued = true;
		s_iNextAllowedFlushMs = now + minIntervalMs;

		Print(string.Format("[DZ-SAVE] commit queued in %1ms reason=%2", delayMs, reason), LogLevel.NORMAL);
		GetGame().GetCallqueue().CallLater(DoGlobalFlush_S, delayMs, false);
	}

	//------------------------------------------------------------------------------------------------
	protected static void DoGlobalFlush_S()
	{
		s_bFlushQueued = false;

		if (TryQueueOverwriteFlushOnce_S())
			return;

		ScheduleRetry_S();
	}

	//------------------------------------------------------------------------------------------------
	protected static bool TryQueueOverwriteFlushOnce_S()
	{
		if (s_bFlushInFlight)
			return true;

		SaveGameManager sgm = GetGame().GetSaveGameManager();
		if (!sgm)
		{
			Print(string.Format("[DZ-SAVE] commit aborted (no SaveGameManager) reason=%1", s_sPendingReason), LogLevel.WARNING);
			InvokeFlushCompleted(s_ePendingSaveType, false, s_sPendingReason);
			s_sPendingReason = string.Empty;
			return false;
		}

		if (!sgm.IsSavingAllowed() || !sgm.IsSavingPossible())
		{
			Print(string.Format("[DZ-SAVE] commit blocked (not allowed/possible) reason=%1", s_sPendingReason), LogLevel.WARNING);
			return false;
		}

		if (sgm.IsBusy())
		{
			Print(string.Format("[DZ-SAVE] commit busy reason=%1", s_sPendingReason), LogLevel.WARNING);
			return false;
		}

		SaveGameOperationCb cb = new SaveGameOperationCb(DZ_OnSaveGameCommitFinished);

		s_bFlushInFlight = true;
		s_sInFlightReason = s_sPendingReason;
		s_eInFlightSaveType = s_ePendingSaveType;

		bool ok = DZ_SaveGameUtil.OverwriteLatestSave(sgm, cb);
		if (!ok)
		{
			Print(string.Format("[DZ-SAVE] overwrite skipped/failed (no active savepoint yet?) reason=%1", s_sInFlightReason), LogLevel.WARNING);
			s_bFlushInFlight = false;
			InvokeFlushCompleted(s_eInFlightSaveType, false, s_sInFlightReason);
			s_sPendingReason = string.Empty;
		}
		else
		{
			Print(string.Format("[DZ-SAVE] overwrite requested=1 reason=%1", s_sInFlightReason), LogLevel.NORMAL);
		}

		return ok;
	}

	//------------------------------------------------------------------------------------------------
	protected static void ScheduleRetry_S()
	{
		if (s_bRetryScheduled)
			return;

		s_bRetryScheduled = true;
		s_iRetryCount = 0;

		Print(string.Format("[DZ-SAVE] scheduling retry-loop reason=%1", s_sPendingReason), LogLevel.WARNING);
		GetGame().GetCallqueue().CallLater(RetryTick_S, RETRY_DELAY_MS, false);
	}

	//------------------------------------------------------------------------------------------------
	protected static void RetryTick_S()
	{
		if (!Replication.IsServer())
		{
			s_bRetryScheduled = false;
			return;
		}

		if (s_bFlushInFlight)
		{
			s_bRetryScheduled = false;
			return;
		}

		s_iRetryCount++;

		if (s_iRetryCount > MAX_RETRIES)
		{
			Print(string.Format("[DZ-SAVE] retry-loop exhausted (%1) reason=%2", MAX_RETRIES, s_sPendingReason), LogLevel.ERROR);
			s_bRetryScheduled = false;
			InvokeFlushCompleted(s_ePendingSaveType, false, s_sPendingReason);
			s_sPendingReason = string.Empty;
			return;
		}

		if (TryQueueOverwriteFlushOnce_S())
		{
			s_bRetryScheduled = false;
			return;
		}

		GetGame().GetCallqueue().CallLater(RetryTick_S, RETRY_DELAY_MS, false);
	}

	//------------------------------------------------------------------------------------------------
	protected static void DZ_OnSaveGameCommitFinished(bool success)
	{
		string reason = s_sInFlightReason;
		ESaveGameType saveType = s_eInFlightSaveType;

		s_bFlushInFlight = false;
		s_sInFlightReason = string.Empty;
		s_eInFlightSaveType = ESaveGameType.MANUAL;

		if (success)
			Print(string.Format("[DZ-SAVE] overwrite finished success=1 reason=%1", reason), LogLevel.NORMAL);
		else
			Print(string.Format("[DZ-SAVE] overwrite finished success=0 reason=%1", reason), LogLevel.WARNING);

		InvokeFlushCompleted(saveType, success, reason);

		if (s_sPendingReason == reason)
			s_sPendingReason = string.Empty;

		if (!s_sPendingReason.IsEmpty())
			GetGame().GetCallqueue().CallLater(DoGlobalFlush_S, 250, false);
	}

	//------------------------------------------------------------------------------------------------
	protected static void InvokeFlushCompleted(ESaveGameType saveType, bool success, string reason)
	{
		if (!s_OnFlushCompleted)
			return;

		s_OnFlushCompleted.Invoke(saveType, success, reason);
	}
}

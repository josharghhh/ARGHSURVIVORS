modded class SCR_BaseGameMode
{
	[Attribute("600", desc: "Overwrite interval in seconds. Performs overwrite-only commits.")]
	protected int m_iDZ_OverwriteIntervalSec;

	[Attribute("30", desc: "Days worth of overwrites before creating a new hard savepoint.")]
	protected int m_iDZ_DaysPerHardSavepoint;

	protected bool m_bDZ_LoopEnabled;

	protected bool m_bDZ_InitialSaveRequested = false;
	protected bool m_bDZ_InitialSaveCreated = false;

	protected int m_iDZ_OverwriteCounter = 0;
	protected int m_iDZ_OverwritesPerHard = 0;

	protected int m_iDZ_SavepointIndex = 0;

	override protected void OnGameStart()
	{
		super.OnGameStart();

		if (!Replication.IsServer())
			return;

		DZ_RecomputeOverwriteBudget_S();

		GetGame().GetCallqueue().CallLater(DZ_ConfigureSaveSystem_S, 200, false);
		GetGame().GetCallqueue().CallLater(DZ_WarmupSaveCache_S, 400, false);

		GetGame().GetCallqueue().CallLater(DZ_EnsureInitialSavePoint_S, 1500, false);
		GetGame().GetCallqueue().CallLater(DZ_StartOverwriteLoop_S, 2500, false);
	}

	override void OnGameEnd()
	{
		m_bDZ_LoopEnabled = false;

		GetGame().GetCallqueue().Remove(DZ_OverwriteTick_S);

		super.OnGameEnd();
	}

	protected void DZ_RecomputeOverwriteBudget_S()
	{
		int intervalSec = m_iDZ_OverwriteIntervalSec;
		if (intervalSec < 60)
			intervalSec = 60;

		int days = m_iDZ_DaysPerHardSavepoint;
		if (days < 1)
			days = 1;

		int totalSeconds = days * 24 * 60 * 60;

		m_iDZ_OverwritesPerHard = totalSeconds / intervalSec;

		if (m_iDZ_OverwritesPerHard < 1)
			m_iDZ_OverwritesPerHard = 1;

		Print(string.Format("[DZ-SAVE] OverwriteBudget: interval=%1s days=%2 -> overwritesPerHard=%3", intervalSec, days, m_iDZ_OverwritesPerHard), LogLevel.WARNING);
	}

	protected void DZ_ConfigureSaveSystem_S()
	{
		SaveGameManager sgm = GetGame().GetSaveGameManager();
		if (!sgm)
			return;

		sgm.SetEnabledSaveTypes(ESaveGameType.MANUAL);
		sgm.SetSavingAllowed(true);

		Print("[DZ-SAVE] SaveGameManager configured: enabled=MANUAL (overwrite cadence owned by ARGHSURVIVORS)", LogLevel.WARNING);
	}

	protected void DZ_WarmupSaveCache_S()
	{
		SaveGameManager sgm = GetGame().GetSaveGameManager();
		if (!sgm)
			return;

		sgm.RetrieveSaveGameInfo(null, null);
	}

	protected void DZ_EnsureInitialSavePoint_S()
	{
		if (m_bDZ_InitialSaveCreated || m_bDZ_InitialSaveRequested)
			return;

		SaveGameManager sgm = GetGame().GetSaveGameManager();
		if (!sgm)
			return;

		sgm.RetrieveSaveGameInfo(null, null);

		array<SaveGame> saves = {};
		int count = sgm.GetSaves(saves);

		if (count > 0)
		{
			m_bDZ_InitialSaveCreated = true;
			Print(string.Format("[DZ-SAVE] Initial save exists (count=%1) -> overwrite loop allowed", count), LogLevel.NORMAL);
			return;
		}

		if (!sgm.IsSavingPossible() || !sgm.IsSavingAllowed() || sgm.IsBusy())
		{
			GetGame().GetCallqueue().CallLater(DZ_EnsureInitialSavePoint_S, 1500, false);
			return;
		}

		m_bDZ_InitialSaveRequested = true;

		Print("[DZ-SAVE] Creating INITIAL savepoint (MANUAL bootstrap)", LogLevel.WARNING);

		SaveGameOperationCb cb = new SaveGameOperationCb(DZ_OnInitialSaveCreated_S);
		sgm.RequestSavePoint(ESaveGameType.MANUAL, "Initial", 0, cb);
	}

	protected void DZ_OnInitialSaveCreated_S(bool success)
	{
		m_bDZ_InitialSaveRequested = false;

		if (!success)
		{
			Print("[DZ-SAVE] Initial savepoint creation FAILED -> retry", LogLevel.WARNING);
			GetGame().GetCallqueue().CallLater(DZ_EnsureInitialSavePoint_S, 2000, false);
			return;
		}

		m_bDZ_InitialSaveCreated = true;

		SaveGameManager sgm = GetGame().GetSaveGameManager();
		if (sgm)
			sgm.RetrieveSaveGameInfo(null, null);

		Print("[DZ-SAVE] Initial savepoint created -> overwrite loop allowed", LogLevel.NORMAL);
	}

	protected void DZ_StartOverwriteLoop_S()
	{
		if (m_iDZ_OverwriteIntervalSec <= 0)
		{
			Print("[DZ-SAVE] overwrite loop disabled (interval <= 0)", LogLevel.WARNING);
			return;
		}

		m_bDZ_LoopEnabled = true;

		int intervalMs = m_iDZ_OverwriteIntervalSec * 1000;
		GetGame().GetCallqueue().CallLater(DZ_OverwriteTick_S, intervalMs, true);

		Print(string.Format("[DZ-SAVE] overwrite loop started interval=%1s (hard savepoint every %2 overwrites)", m_iDZ_OverwriteIntervalSec, m_iDZ_OverwritesPerHard), LogLevel.WARNING);
	}

	protected void DZ_OverwriteTick_S()
	{
		if (!m_bDZ_LoopEnabled)
			return;

		if (!m_bDZ_InitialSaveCreated)
		{
			Print("[DZ-SAVE] overwrite tick skipped (initial save not ready)", LogLevel.NORMAL);
			DZ_EnsureInitialSavePoint_S();
			return;
		}

		DZ_PersistenceCommit.RequestGlobalFlush_S("overwrite_tick", 15000, 0, ESaveGameType.MANUAL);

		m_iDZ_OverwriteCounter++;

		if (m_iDZ_OverwriteCounter >= m_iDZ_OverwritesPerHard)
		{
			m_iDZ_OverwriteCounter = 0;
			DZ_CreateHardSavepoint_S();
		}
	}

	protected void DZ_CreateHardSavepoint_S()
	{
		SaveGameManager sgm = GetGame().GetSaveGameManager();
		if (!sgm)
			return;

		if (!sgm.IsSavingPossible() || !sgm.IsSavingAllowed() || sgm.IsBusy())
		{
			Print("[DZ-SAVE] hard savepoint skipped (sgm not ready)", LogLevel.WARNING);
			return;
		}

		m_iDZ_SavepointIndex++;

		string name = string.Format("ARGH_HARD_%1_%2", m_iDZ_SavepointIndex, System.GetTickCount());

		Print(string.Format("[DZ-SAVE] Creating HARD savepoint (MANUAL) name=%1", name), LogLevel.WARNING);

		SaveGameOperationCb cb = new SaveGameOperationCb(DZ_OnHardSavepointCreated_S);
		sgm.RequestSavePoint(ESaveGameType.MANUAL, name, 0, cb);
	}

	protected void DZ_OnHardSavepointCreated_S(bool success)
	{
		if (success)
			Print("[DZ-SAVE] hard savepoint created success=1", LogLevel.WARNING);
		else
			Print("[DZ-SAVE] hard savepoint created success=0", LogLevel.ERROR);

		SaveGameManager sgm = GetGame().GetSaveGameManager();
		if (sgm)
			sgm.RetrieveSaveGameInfo(null, null);
	}
}

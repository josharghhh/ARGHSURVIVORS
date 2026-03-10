class ARGH_SpawnDialog : SCR_ConfigurableDialogUi
{
	static const ResourceName BASE_LAYOUT = "{604721ED8E6ED458}UI/layouts/Menus/Dialogs/ConfigurableDialog_Medium.layout";
	static const ResourceName CONTENT_LAYOUT = "{A9DB0F9A8D814C21}UI/layouts/Menus/Dialogs/ARGH_SpawnDialogContent.layout";

	protected static ref ARGH_SpawnDialog s_CurrentDialog;

	protected string m_sStatus;
	protected string m_sHint;
	protected bool m_bRetryMode;
	protected bool m_bPassiveMode;
	protected ref Color m_cStatusColor = Color.White;

	protected TextWidget m_wSpawnStatus;
	protected TextWidget m_wSpawnHint;
	protected TextWidget m_wSpawnTitle;

	//------------------------------------------------------------------------------------------------
	static ARGH_SpawnDialog OpenDialog(string status, string hint, bool retryMode)
	{
		return OpenInternal(status, hint, retryMode, false);
	}

	//------------------------------------------------------------------------------------------------
	static ARGH_SpawnDialog OpenPassiveDialog(string status, string hint)
	{
		return OpenInternal(status, hint, false, true);
	}

	//------------------------------------------------------------------------------------------------
	static void UpdateCurrent(string status, string hint, bool retryMode, bool passiveMode)
	{
		if (!s_CurrentDialog)
		{
			OpenInternal(status, hint, retryMode, passiveMode);
			return;
		}

		s_CurrentDialog.Configure(status, hint, retryMode, passiveMode);
	}

	//------------------------------------------------------------------------------------------------
	static void SetCurrentStatusColor(Color color)
	{
		if (!s_CurrentDialog)
			return;

		s_CurrentDialog.SetStatusColor(color);
	}

	//------------------------------------------------------------------------------------------------
	protected static ARGH_SpawnDialog OpenInternal(string status, string hint, bool retryMode, bool passiveMode)
	{
		if (s_CurrentDialog)
			s_CurrentDialog.Close();

		ARGH_SpawnDialog dialog = new ARGH_SpawnDialog();
		SCR_ConfigurableDialogUiPreset preset = BuildPreset(retryMode, passiveMode);
		ARGH_SpawnDialog created = ARGH_SpawnDialog.Cast(CreateByPreset(preset, dialog));
		if (!created)
			return null;

		created.Configure(status, hint, retryMode, passiveMode);
		s_CurrentDialog = created;
		return created;
	}

	//------------------------------------------------------------------------------------------------
	static void CloseCurrent()
	{
		if (!s_CurrentDialog)
			return;

		s_CurrentDialog.Close();
		s_CurrentDialog = null;
	}

	//------------------------------------------------------------------------------------------------
	protected static SCR_ConfigurableDialogUiPreset BuildPreset(bool retryMode, bool passiveMode)
	{
		SCR_ConfigurableDialogUiPreset preset = new SCR_ConfigurableDialogUiPreset();
		preset.m_sLayout = BASE_LAYOUT;
		preset.m_sContentLayout = CONTENT_LAYOUT;
		preset.m_sTag = "argh_spawn_dialog";
		preset.m_bShowIcon = false;
		preset.m_sTitle = "";
		preset.m_sMessage = "";
		preset.m_aButtons = {};
		if (retryMode)
			preset.m_eVisualStyle = EDialogType.WARNING;
		else
			preset.m_eVisualStyle = EDialogType.ACTION;

		if (passiveMode)
			return preset;

		SCR_ConfigurableDialogUiButtonPreset confirmButton = new SCR_ConfigurableDialogUiButtonPreset();
		confirmButton.m_sTag = BUTTON_CONFIRM;
		confirmButton.m_sActionName = "DialogConfirm";
		confirmButton.m_eAlign = EConfigurableDialogUiButtonAlign.CENTER;
		confirmButton.m_bShowButton = true;
		if (retryMode)
			confirmButton.m_sLabel = "retry spawn";
		else
			confirmButton.m_sLabel = "spawn";
		preset.m_aButtons.Insert(confirmButton);

		return preset;
	}

	//------------------------------------------------------------------------------------------------
	void Configure(string status, string hint, bool retryMode, bool passiveMode)
	{
		m_sStatus = status;
		m_sHint = hint;
		m_bRetryMode = retryMode;
		m_bPassiveMode = passiveMode;
		RefreshTexts();
	}

	//------------------------------------------------------------------------------------------------
	void SetStatusColor(Color color)
	{
		if (color)
			m_cStatusColor = color;
		else
			m_cStatusColor = Color.White;

		if (m_wSpawnStatus)
			m_wSpawnStatus.SetColor(m_cStatusColor);
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnMenuOpen(SCR_ConfigurableDialogUiPreset preset)
	{
		super.OnMenuOpen(preset);
		SetMessage("");

		Widget contentRoot = GetContentLayoutRoot();
		if (!contentRoot)
			contentRoot = GetRootWidget();

		if (contentRoot)
		{
			m_wSpawnTitle = TextWidget.Cast(contentRoot.FindAnyWidget("SpawnTitle"));
			m_wSpawnStatus = TextWidget.Cast(contentRoot.FindAnyWidget("SpawnStatus"));
			m_wSpawnHint = TextWidget.Cast(contentRoot.FindAnyWidget("SpawnHint"));
		}

		if (!m_wSpawnStatus || !m_wSpawnHint)
		{
			Widget rootWidget = GetRootWidget();
			if (rootWidget)
			{
				if (!m_wSpawnTitle)
					m_wSpawnTitle = TextWidget.Cast(rootWidget.FindAnyWidget("SpawnTitle"));
				if (!m_wSpawnStatus)
					m_wSpawnStatus = TextWidget.Cast(rootWidget.FindAnyWidget("SpawnStatus"));
				if (!m_wSpawnHint)
					m_wSpawnHint = TextWidget.Cast(rootWidget.FindAnyWidget("SpawnHint"));
			}
		}

		RefreshTexts();
	}

	//------------------------------------------------------------------------------------------------
	override void OnMenuClose()
	{
		super.OnMenuClose();

		if (s_CurrentDialog == this)
			s_CurrentDialog = null;
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnConfirm()
	{
		if (m_bPassiveMode)
			return;

		SCR_InputButtonComponent confirmButton = FindButton(BUTTON_CONFIRM);
		if (confirmButton)
			confirmButton.SetEnabled(false);

		if (m_bRetryMode)
		{
			Configure("searching the coastline for a valid spawn.", "please wait while argh survivors retries your deployment.", true, false);
		}
		else
		{
			Configure("deploying survivor.", "please wait while your coastal spawn finalizes.", false, false);
		}

		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		ARGH_RespawnMode mode = ARGH_RespawnMode.None;
		if (playerController)
			mode = playerController.ARGH_GetRespawnMode_O();

		Print(string.Format("[ARGH_PERSIST] action=spawn-dialog-confirm retry=%1 mode='%2' has-controller=%3", m_bRetryMode, typename.EnumToString(ARGH_RespawnMode, mode), playerController != null), LogLevel.WARNING);

		if (playerController)
		{
			if (!m_bRetryMode && playerController.ARGH_GetRespawnMode_O() == ARGH_RespawnMode.InitialJoin)
			{
				// Initial first-join path: always route spawn confirmation through server logic.
				// This avoids client-only hidden auto-deploy loops when local CanSpawn stays denied.
				playerController.ARGH_RequestSpawnDialogAction(false);
				Print("[ARGH_PERSIST] action=spawn-dialog-initial-join-request routed='server-confirm-rpc'", LogLevel.WARNING);
				return;
			}

			playerController.ARGH_RequestSpawnDialogAction(m_bRetryMode);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void RefreshTexts()
	{
		SetMessage("");

		if (m_wSpawnTitle)
			m_wSpawnTitle.SetText("ARGH SURVIVORS");

		if (m_wSpawnStatus)
		{
			m_wSpawnStatus.SetText(m_sStatus);
			m_wSpawnStatus.SetColor(m_cStatusColor);
		}

		if (m_wSpawnHint)
			m_wSpawnHint.SetText(m_sHint);

		// Safety fallback: if content widgets fail to resolve in this menu preset,
		// still show status/hint in base message area instead of blank dialog.
		if (!m_wSpawnStatus && !m_wSpawnHint)
			SetMessage(m_sStatus + "\n" + m_sHint);

		// Do not fallback to base dialog message text.
		// We intentionally render only layout widgets so custom fonts/styles always apply.
	}

}

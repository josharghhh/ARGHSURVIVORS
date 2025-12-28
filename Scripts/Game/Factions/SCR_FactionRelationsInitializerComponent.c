[ComponentEditorProps(category: "GameScripted/Relations", description: "Sets predefined hostile relations once factions are registered.")]
class SCR_FactionRelationsInitializerComponentClass : ScriptComponentClass {}

class SCR_FactionRelationsInitializerComponent : ScriptComponent
{
		protected int m_iRetryCount;
		protected static const int MAX_RETRIES = 10;
		protected SCR_FactionManager m_FactionManager;

		[Attribute(defvalue: "SURVIVORS", uiwidget: UIWidgets.EditBox, desc: "Faction key that should be hostile to the listed targets.")]
		protected string m_sSourceFactionKey;

		[Attribute(defvalue: "ARGH,BALLIEN_BC_FACTION,BACON_622120A5448725E3_FACTION", uiwidget: UIWidgets.EditBox, desc: "Comma-separated faction keys to set as hostile targets.")]
		protected string m_sTargetFactionKeys;

		[Attribute(defvalue: "0", uiwidget: UIWidgets.CheckBox, desc: "If set, source becomes hostile to every other registered faction (ignores target list).")]
		protected bool m_bHostileToAllFactions;

		[Attribute(defvalue: "1", uiwidget: UIWidgets.CheckBox, desc: "If set, also mirrors hostility from each target back to the source.")]
		protected bool m_bBidirectional;

	//------------------------------------------------------------------------------------------------
		override void EOnInit(IEntity owner)
		{
			// Run everywhere so it also works in Workbench preview; guard is inside.
			// Repeat a few times because factions may register a bit later in Workbench/GM.
			GetGame().GetCallqueue().CallLater(InitRelations, 1000, true, owner);
		}

		//------------------------------------------------------------------------------------------------
	protected void InitRelations(IEntity owner)
	{
		m_FactionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
		if (!m_FactionManager)
			return;

		SCR_Faction source = SCR_Faction.Cast(m_FactionManager.GetFactionByKey(m_sSourceFactionKey));
		if (!source)
			return;

		array<string> targetKeys = CollectTargetKeys();

		if (targetKeys.IsEmpty())
		{
			// Factions may not be registered yet in Workbench; retry a few times.
			m_iRetryCount++;
			if (m_iRetryCount > MAX_RETRIES)
				GetGame().GetCallqueue().Remove(InitRelations);
			return;
		}

		ApplyHostility(source, targetKeys);

		SCR_FactionManager.RequestUpdateAllTargetsFactions();
		string targetsInfo;
		if (m_bHostileToAllFactions)
			targetsInfo = "ALL REGISTERED FACTIONS";
		else
			targetsInfo = m_sTargetFactionKeys;

		Print(string.Format("[RelationsInit] Hostility set: %1 vs %2 (bidirectional=%3)", m_sSourceFactionKey, targetsInfo, m_bBidirectional));

		// Stop the repeating call once we've successfully applied relations.
		GetGame().GetCallqueue().Remove(InitRelations);
	}

		//------------------------------------------------------------------------------------------------
	protected array<string> CollectTargetKeys()
	{
		array<string> targetKeys = {};

		if (m_bHostileToAllFactions)
		{
			array<Faction> factions = {};
			m_FactionManager.GetFactionsList(factions);

			foreach (Faction faction : factions)
			{
				SCR_Faction scriptedFaction = SCR_Faction.Cast(faction);
				if (!scriptedFaction)
					continue;

				string key = scriptedFaction.GetFactionKey();
				if (key == m_sSourceFactionKey || key.IsEmpty())
					continue;

				targetKeys.Insert(key);
			}
		}
		else
		{
			m_sTargetFactionKeys.Split(",", targetKeys, false);
		}

		return targetKeys;
	}

	//------------------------------------------------------------------------------------------------
	protected void ApplyHostility(SCR_Faction source, notnull array<string> targetKeys)
	{
		foreach (string rawKey : targetKeys)
		{
			string key = rawKey;
			key.Replace(" ", "");
			if (key.IsEmpty())
				continue;

			SCR_Faction target = SCR_Faction.Cast(m_FactionManager.GetFactionByKey(key));
			if (!target)
				continue;

			m_FactionManager.SetFactionsHostile(source, target, -1, false);
			if (m_bBidirectional)
				m_FactionManager.SetFactionsHostile(target, source, -1, false);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void OnFactionRegistered(SCR_Faction faction)
	{
		if (!faction || faction.GetFactionKey() == m_sSourceFactionKey || !m_FactionManager)
			return;

		array<string> targetKeys = {};

		if (m_bHostileToAllFactions)
		{
			targetKeys.Insert(faction.GetFactionKey());
		}
		else
		{
			m_sTargetFactionKeys.Split(",", targetKeys, false);
			bool match = false;
			foreach (string rawKey : targetKeys)
			{
				string key = rawKey;
				key.Replace(" ", "");
				if (key == faction.GetFactionKey())
				{
					match = true;
					break;
				}
			}

			if (!match)
				return;

			targetKeys.Clear();
			targetKeys.Insert(faction.GetFactionKey());
		}

		SCR_Faction source = SCR_Faction.Cast(m_FactionManager.GetFactionByKey(m_sSourceFactionKey));
		if (!source)
			return;

		ApplyHostility(source, targetKeys);
		SCR_FactionManager.RequestUpdateAllTargetsFactions();
		Print(string.Format("[RelationsInit] Hostility refreshed for newly registered faction %1 vs %2", m_sSourceFactionKey, faction.GetFactionKey()));
	}
}

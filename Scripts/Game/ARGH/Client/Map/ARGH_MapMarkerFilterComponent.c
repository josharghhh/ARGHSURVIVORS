class ARGH_MapMarkerFilterComponentClass : ScriptComponentClass
{
}

class ARGH_MapMarkerFilterComponent : ScriptComponent
{
	protected const int FILTER_INTERVAL_MS = 1000;

	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);

		if (Replication.IsServer())
			return;

		if (!GetGame())
			return;

		GetGame().GetCallqueue().Remove(ARGH_FilterMarkers);
		GetGame().GetCallqueue().CallLater(ARGH_FilterMarkers, FILTER_INTERVAL_MS, true);
	}

	void ~ARGH_MapMarkerFilterComponent()
	{
		if (!GetGame())
			return;

		GetGame().GetCallqueue().Remove(ARGH_FilterMarkers);
	}

	protected void ARGH_FilterMarkers()
	{
		SCR_MapMarkerManagerComponent markerManager = SCR_MapMarkerManagerComponent.GetInstance();
		if (!markerManager)
			return;

		array<SCR_MapMarkerEntity> dynamicMarkers = markerManager.GetDynamicMarkers();
		if (!dynamicMarkers)
			return;

		foreach (SCR_MapMarkerEntity marker : dynamicMarkers)
		{
			if (!marker)
				continue;

			if (!ARGH_ShouldHideMarker(marker))
				continue;

			marker.SetLocalVisible(false);
		}
	}

	protected bool ARGH_ShouldHideMarker(SCR_MapMarkerEntity marker)
	{
		Faction markerFaction = marker.GetFaction();
		if (markerFaction && ARGH_ShouldHideFaction(markerFaction.GetFactionKey()))
			return true;

		IEntity target = marker.GetTarget();
		if (!target)
			return false;

		FactionAffiliationComponent factionComponent = FactionAffiliationComponent.Cast(target.FindComponent(FactionAffiliationComponent));
		if (!factionComponent)
			return false;

		string factionKey = factionComponent.GetAffiliatedFactionKey();
		if (factionKey.IsEmpty())
			return false;

		return ARGH_ShouldHideFaction(factionKey);
	}

	protected bool ARGH_ShouldHideFaction(string factionKey)
	{
		return factionKey == "FIA" || factionKey == "USSR" || factionKey == "SCAV";
	}
}

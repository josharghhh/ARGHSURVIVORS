modded class MO_InfilUserAction
{
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		string areaName = GetActiveContext().GetName();
		MO_GameModeStateSystem stateSystem = MO_GameModeStateSystem.GetInstance();
		if (!stateSystem)
			return;

		// Ensure spawn areas are initialized on the local side.
		if (!stateSystem.spawnAreas)
			stateSystem.InitScavSpawns();

		if (!stateSystem.spawnAreas)
			return;
		
		MO_Spawn_Area spawnArea = stateSystem.spawnAreas.Get(areaName);
		if (!spawnArea)
		{
			PrintFormat("MO_InflilAction.PerformAction(): No spawn areas found called '%1' of %2 registered areas", areaName, stateSystem.spawnAreas.Count());
			return;
		}
		
		IEntity child = spawnArea.GetChildren();
		if (!child)
		{
			PrintFormat("MO_InflilAction.PerformAction(): No spawn positions in spawn area '%1'", areaName);
			return;
		}
		
		array<IEntity> spawns = {};
		while (child)
		{
			spawns.Insert(child);
			IEntity sibling = child.GetSibling();
			if (sibling)
				child = sibling;
			else
				child = null;
		}
		
		IEntity spawn = spawns.GetRandomElement();
		
		SCR_SpawnPositionComponent sc = SCR_SpawnPositionComponent.Cast(spawn.FindComponent(SCR_SpawnPositionComponent));
		SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!pc)
			return;
		
		SCR_CampaignFastTravelComponent fastTravel = SCR_CampaignFastTravelComponent.Cast(pc.FindComponent(SCR_CampaignFastTravelComponent));
		if (!fastTravel)
		{
			PrintFormat("MO_InflilAction.PerformAction(): SCR_PlayerController is missing SCR_CampaignFastTravelComponent!");
			return;
		}
		
		isTravelling = true;
		fastTravel.SetDestination(fastTravel.FindDestinationId(spawn), areaName);
		fastTravel.Deploy();
		
		GetGame().GetCallqueue().CallLater(ResetIsTravelling, 10000);
	}
}

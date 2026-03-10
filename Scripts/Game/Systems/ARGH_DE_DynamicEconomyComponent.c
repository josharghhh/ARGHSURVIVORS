// Harden Dynamic Economy player sync against missing/late resource containers.
modded class DE_DynamicEconomyComponent
{
	protected static const int ARGH_MAX_SYNC_ATTEMPTS = 10;

	override void UpdatePlayerData(int playerId, int attempts = 0)
	{
		SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!pc)
			return;

		DE_EconomySystem deSystem = DE_EconomySystem.GetInstance();
		if (!deSystem)
		{
			ARGH_QueueUpdatePlayerDataRetry(playerId, attempts);
			return;
		}

		// Sync trader rep values.
		UUID playerUuid = SCR_PlayerIdentityUtils.GetPlayerIdentityId(playerId);
		if (deSystem.traders)
		{
			foreach (DE_TraderEntity trader : deSystem.traders)
			{
				if (!trader)
					continue;

				float rep = trader.GetRep(playerUuid);
				if (rep)
					pc.NotifyRepChange(Replication.FindId(trader), rep);
			}
		}

		// Bank account lives on player controller.
		SCR_ResourceComponent playerResource = SCR_ResourceComponent.Cast(pc.FindComponent(SCR_ResourceComponent));
		if (!playerResource)
		{
			ARGH_QueueUpdatePlayerDataRetry(playerId, attempts);
			return;
		}

		SCR_ResourceContainer playerContainer = playerResource.GetContainer(EResourceType.CASH);
		if (!playerContainer)
		{
			ARGH_QueueUpdatePlayerDataRetry(playerId, attempts);
			return;
		}

		pc.NotifyBankDataChange(Replication.FindId(pc), playerContainer.GetResourceValue());

		// Wallet lives on controlled character.
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(pc.GetControlledEntity());
		if (!character)
		{
			ARGH_QueueUpdatePlayerDataRetry(playerId, attempts);
			return;
		}

		SCR_ResourceComponent characterResource = SCR_ResourceComponent.Cast(character.FindComponent(SCR_ResourceComponent));
		if (!characterResource)
		{
			ARGH_QueueUpdatePlayerDataRetry(playerId, attempts);
			return;
		}

		SCR_ResourceContainer characterContainer = characterResource.GetContainer(EResourceType.CASH);
		if (!characterContainer)
		{
			ARGH_QueueUpdatePlayerDataRetry(playerId, attempts);
			return;
		}

		pc.NotifyBankDataChange(Replication.FindId(character), characterContainer.GetResourceValue());
	}

	protected void ARGH_QueueUpdatePlayerDataRetry(int playerId, int attempts)
	{
		if (attempts >= ARGH_MAX_SYNC_ATTEMPTS)
			return;

		DE_EconomySystem deSystem = DE_EconomySystem.GetInstance();
		if (!deSystem || !deSystem.callQueue)
			return;

		deSystem.callQueue.CallLater(UpdatePlayerData, 1000, param1: playerId, param2: attempts + 1);
	}
}

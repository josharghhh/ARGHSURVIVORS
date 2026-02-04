// Ensure bank + wallet values are pushed to the client after spawn/respawn.
modded class SCR_PlayerController
{
	override void OnControlledEntityChanged(IEntity from, IEntity to)
	{
		super.OnControlledEntityChanged(from, to);
		if (!Replication.IsServer())
			return;
		
		// Delay slightly so PC + character resources exist and are replicated.
		GetGame().GetCallqueue().CallLater(ARGH_SyncBankWallet, 500, false);
	}

	protected void ARGH_SyncBankWallet()
	{
		if (!Replication.IsServer())
			return;
		
		// Bank lives on player controller.
		SCR_ResourceComponent pcRes = SCR_ResourceComponent.Cast(FindComponent(SCR_ResourceComponent));
		if (pcRes)
		{
			SCR_ResourceContainer bank = pcRes.GetContainer(EResourceType.CASH);
			if (bank)
				NotifyBankDataChange(Replication.FindId(this), bank.GetResourceValue());
		}
		
		// Wallet lives on controlled character.
		IEntity characterEnt = GetGame().GetPlayerManager().GetPlayerControlledEntity(GetPlayerId());
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(characterEnt);
		if (!character)
			return;
		
		SCR_ResourceComponent charRes = SCR_ResourceComponent.Cast(character.FindComponent(SCR_ResourceComponent));
		if (!charRes)
			return;
		
		SCR_ResourceContainer wallet = charRes.GetContainer(EResourceType.CASH);
		if (wallet)
			NotifyBankDataChange(Replication.FindId(character), wallet.GetResourceValue());
	}
}

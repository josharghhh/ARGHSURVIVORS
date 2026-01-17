// -----------------------------------------------------------------------------
// ARGH_DrinkWaterActions.c
// 
// User actions for drinking water from water sources.
// Location: ARGH/Shared/UserActions/
// 
// Handles:
//   - Drinking from water sources (wells, ponds, etc.)
//   - Hydration restoration via RPC to server
// -----------------------------------------------------------------------------

// Modded base class for all drink water actions
modded class FM_DrinkWaterPlayerAction
{
	[Attribute(defvalue: "20.0", desc: "Hydration added per use. Can be negative for dehydration.")]
	protected float m_iWaterValue;
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		ApplyHydrationEffect(pUserEntity);
	}
	
	override void ApplyHydrationEffect(IEntity characterOwner)
	{
		if (!characterOwner)
		{
			Print("[ARGH] DrinkWater: Character not found.");
			return;
		}
		
		IEntity resolvedOwner = characterOwner;
		SCR_PlayerController pc = SCR_PlayerController.Cast(characterOwner);
		if (pc)
		{
			IEntity controlled = pc.GetControlledEntity();
			if (!controlled)
				controlled = pc.GetMainEntity();

			if (controlled)
				resolvedOwner = controlled;
		}

		if (resolvedOwner && !SCR_CharacterControllerComponent.Cast(resolvedOwner.FindComponent(SCR_CharacterControllerComponent)))
		{
			IEntity parent = resolvedOwner.GetParent();
			if (parent)
				resolvedOwner = parent;
		}

		SCR_CharacterControllerComponent metabolismComponent = SCR_CharacterControllerComponent.Cast(resolvedOwner.FindComponent(SCR_CharacterControllerComponent));
		if (metabolismComponent)
		{
			Print("[ARGH] DrinkWater: SCR_CharacterControllerComponent found.");
			
			// Apply hydration changes on server (RPC if needed)
			metabolismComponent.RequestHydrationChange(m_iWaterValue);
			
			float currentHydration = metabolismComponent.GetHydration();
			Print(string.Format("[ARGH] DrinkWater: Increased hydration by %1, Current: %2", m_iWaterValue, currentHydration));
		}
		else
		{
			Print("[ARGH] DrinkWater: SCR_CharacterControllerComponent not found on character.");
		}
	}
}

// Extended class for specific water sources
class ARGH_DrinkWaterPlayerAction : FM_DrinkWaterPlayerAction
{
	override void ApplyHydrationEffect(IEntity characterOwner)
	{
		if (!characterOwner)
			return;

		IEntity resolvedOwner = characterOwner;
		SCR_PlayerController pc = SCR_PlayerController.Cast(characterOwner);
		if (pc)
		{
			IEntity controlled = pc.GetControlledEntity();
			if (!controlled)
				controlled = pc.GetMainEntity();

			if (controlled)
				resolvedOwner = controlled;
		}

		if (resolvedOwner && !SCR_CharacterControllerComponent.Cast(resolvedOwner.FindComponent(SCR_CharacterControllerComponent)))
		{
			IEntity parent = resolvedOwner.GetParent();
			if (parent)
				resolvedOwner = parent;
		}

		SCR_CharacterControllerComponent metabolismComponent = SCR_CharacterControllerComponent.Cast(resolvedOwner.FindComponent(SCR_CharacterControllerComponent));
		if (!metabolismComponent)
			return;

		metabolismComponent.RequestHydrationChange(m_iWaterValue);
	}
}

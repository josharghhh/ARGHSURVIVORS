// File: FM_DrinkWaterPlayerAction.c
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
		if (characterOwner)
		{
			SCR_CharacterControllerComponent metabolismComponent = SCR_CharacterControllerComponent.Cast(characterOwner.FindComponent(SCR_CharacterControllerComponent));
			if (metabolismComponent)
			{
				Print("SCR_CharacterControllerComponent found.");
				
				// Apply hydration changes on server (RPC if needed)
				metabolismComponent.RequestHydrationChange(m_iWaterValue);
				
				float currentHydration = metabolismComponent.GetHydration();
				Print("Increased hydration by " + m_iWaterValue);
				Print("Current Hydration: " + currentHydration);
			}
			else
			{
				Print("SCR_CharacterControllerComponent not found on character.");
			}
		}
		else
		{
			Print("Character not found.");
		}
	}
}

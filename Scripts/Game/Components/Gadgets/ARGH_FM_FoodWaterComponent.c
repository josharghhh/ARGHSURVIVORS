// File: FM_FoodWaterComponent.c
modded class FM_FoodWaterComponentClass
{
}

modded class FM_FoodWaterComponent
{
	// Energy added per use. Can be negative to represent hunger loss.
	[Attribute(defvalue: "0.0", desc: "Energy added per use. Can be negative for hunger loss.")]
	protected float m_iEnergyValue;
	
	// Hydration added per use. Can be negative to represent dehydration.
	[Attribute(defvalue: "0.0", desc: "Hydration added per use. Can be negative for dehydration.")]
	protected float m_iWaterValue;
	
	protected RplComponent m_RplComp;
	protected ChimeraCharacter m_User;
	
	//------------------------------------------------------------------------------------------------
	override void ToggleActive(bool state, SCR_EUseContext context)
	{
		if (m_eUseMask == SCR_EUseContext.NONE || (m_eUseMask & context) == 0)
			return;
		
		if (!m_User)
		{
			m_User = m_CharacterOwner;
			if (!m_User)
				return;
		}
		
		ChimeraCharacter character = m_CharacterOwner;
		if (character)
		{
			SCR_CharacterControllerComponent charController = SCR_CharacterControllerComponent.Cast(character.GetCharacterController());
			if (!charController || charController.GetLifeState() != ECharacterLifeState.ALIVE)
				return;
		}
		
		RplComponent rplComponent = RplComponent.Cast(m_User.FindComponent(RplComponent));
		if (!rplComponent || !rplComponent.IsOwner())
			return;
		
		SCR_GadgetManagerComponent.GetGadgetManager(m_User).AskToggleGadget(this, state);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnToggleActive(bool state)
	{
		if (!m_User)
		{
			if (!m_CharacterOwner)
				return;
			
			m_User = m_CharacterOwner;
			if (!m_User)
				return;
		}
		
		ApplyMetabolismEffect();
		DeleteGadget();
	}
	
	//------------------------------------------------------------------------------------------------
	override void ApplyMetabolismEffect()
	{
		IEntity owner = GetOwner();
		IEntity characterOwner = owner;
		
		if (!SCR_CharacterControllerComponent.Cast(owner.FindComponent(SCR_CharacterControllerComponent)))
		{
			characterOwner = owner.GetParent();
		}
		
		if (characterOwner)
		{
			SCR_CharacterControllerComponent metabolismComponent = SCR_CharacterControllerComponent.Cast(characterOwner.FindComponent(SCR_CharacterControllerComponent));
			if (metabolismComponent)
			{
				metabolismComponent.RequestEnergyChange(m_iEnergyValue);
				metabolismComponent.RequestHydrationChange(m_iWaterValue);
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override void DeleteGadget()
	{
		if (m_CharacterOwner)
		{
			SCR_EntityHelper.DeleteEntityAndChildren(GetOwner());
			m_bActivated = false;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override float GetEnergyValue()
	{
		return m_iEnergyValue;
	}
	
	override float GetWaterValue()
	{
		return m_iWaterValue;
	}
	
	//------------------------------------------------------------------------------------------------
	override void ActivateAction(notnull IEntity pUserEntity)
	{
		m_User = ChimeraCharacter.Cast(pUserEntity);
		ActivateAction();
	}
	
	protected override void ActivateAction()
	{
		ToggleActive(false, SCR_EUseContext.FROM_ACTION);
	}
	
	//------------------------------------------------------------------------------------------------
	override EGadgetType GetType()
	{
		return EGadgetType.CONSUMABLE;
	}
	
	override bool CanBeRaised()
	{
		return true;
	}
	
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT);
	}
}

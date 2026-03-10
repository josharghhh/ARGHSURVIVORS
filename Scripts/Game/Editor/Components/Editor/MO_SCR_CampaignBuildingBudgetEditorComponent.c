class MO_SCR_CampaignBuildingBudgetEditorComponentClass : SCR_CampaignBuildingBudgetEditorComponentClass
{
}

class MO_SCR_CampaignBuildingBudgetEditorComponent : SCR_CampaignBuildingBudgetEditorComponent
{
	// Disable rank budgets as military ranks don't make sense in FFA groups.
	protected ref array<EEditableEntityBudget> m_DisabledBudgets = {
		EEditableEntityBudget.RANK_RENEGADE,
		EEditableEntityBudget.RANK_PRIVATE,
		EEditableEntityBudget.RANK_CORPORAL,
		EEditableEntityBudget.RANK_SERGEANT,
		EEditableEntityBudget.RANK_LIEUTENANT,
		EEditableEntityBudget.RANK_CAPTAIN,
		EEditableEntityBudget.RANK_MAJOR,
		EEditableEntityBudget.RANK_COLONEL,
		EEditableEntityBudget.RANK_GENERAL
	};

	override bool CanEstablishBase()
	{
		if (!m_CampaignBuildingComponent)
			return false;

		SCR_CampaignBuildingProviderComponent providerComponent = m_CampaignBuildingComponent.GetProviderComponent();
		if (!providerComponent)
			return false;

		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		if (!player)
			return false;

		// @TODO add group limit check
		return true;
	}

	protected void FilterDisabledBudgets(inout notnull array<ref SCR_EntityBudgetValue> budgetCosts)
	{
		for (int i = budgetCosts.Count() - 1; i >= 0; i--)
		{
			SCR_EntityBudgetValue budget = budgetCosts[i];
			if (m_DisabledBudgets.Contains(budget.GetBudgetType()))
				budgetCosts.Remove(i);
		}
	}

	override void FilterAvailableBudgets(inout notnull array<ref SCR_EntityBudgetValue> budgetCosts)
	{
		super.FilterAvailableBudgets(budgetCosts);
		FilterDisabledBudgets(budgetCosts);
	}

	override void FilterAvailableBudgetsDontDiscardCampaignBudget(inout notnull array<ref SCR_EntityBudgetValue> budgetCosts)
	{
		super.FilterAvailableBudgetsDontDiscardCampaignBudget(budgetCosts);
		FilterDisabledBudgets(budgetCosts);
	}
}
